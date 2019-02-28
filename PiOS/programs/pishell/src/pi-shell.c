// engler: trivial shell for our pi system.  it's a good strand of yarn
// to pull to motivate the subsequent pieces we do.
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#include <boot-messages.h>

#include <x86/tty.h>
#include <x86/tty-comm.h>
#include <x86/boot-support.h>
#include <x86/boot-send.h>

#include "pi-shell.h"
#include "demand.h"

// have pi send this back when it reboots (otherwise my-install exits).
static const char pi_done[] = "PI REBOOT!!!\n";
// pi sends this after a program executes to indicate it finished.
static const char cmd_done[] = "CMD-DONE\n";


/************************************************************************
 * provided support code.
 */
static void write_exact(int fd, const void* buf, int nbytes) {
    int n;
    if ((n = write(fd, buf, nbytes)) < 0) {
        panic("i/o error writing to pi = <%s>.  Is pi connected?\n",
              strerror(errno));
    }
    demand(n == nbytes, something
            is
            wrong);
}

// write characters to the pi.
static void pi_put(int fd, const char* buf) {
    int n = strlen(buf);
    demand(n, sending
            0
            byte
            string);
    write_exact(fd, buf, n);
}

// read characters from the pi until we see a newline.
int pi_readline(int fd, char* buf, unsigned sz) {
    for (int i = 0; i < sz; i++) {
        int n;
        if ((n = read(fd, &buf[i], 1)) != 1) {
            //note("got %s res=%d, expected 1 byte\n", strerror(n), n);
            return 0;
//            note("assuming: pi connection closed.  cleaning up\n");
//            exit(0);
        }
        if (buf[i] == '\n') {
            buf[i] = 0;
            return 1;
        }
    }
    panic("too big!\n");
}


#define expect_val(fd, v) (expect_val)(fd, v, #v)

static void (expect_val)(int fd, unsigned v, const char* s) {
    unsigned got = tty_get_uint(fd);
    if (v != got)
        panic("expected %s (%x), got: %x\n", s, v, got);
}

// print out argv contents.
static void print_args(const char* msg, char* argv[], int nargs) {
    note("%s: prog=<%s> ", msg, argv[0]);
    for (int i = 1; i < nargs; i++)
        printf("<%s> ", argv[i]);
    printf("\n");
}

// anything with a ".bin" suffix is a pi program.
static int is_pi_prog(char* prog) {
    int n = strlen(prog);

    // must be .bin + at least one character.
    if (n < 5)
        return 0;
    return strcmp(prog + n - 4, ".bin") == 0 || (n > 6 && strcmp(prog + n - 7, "run.cmd") == 0);
}


/***********************************************************************
 * implement the rest.
 */

// catch control c: set done=1 when happens.  
static sig_atomic_t done = 0;

static void on_control_c(int sig) {
    done = 1;
}

static void catch_control_c() {
    struct sigaction act;

    act.sa_handler = on_control_c;
    sigemptyset(&act.sa_mask);
    sigaction(SIGINT, &act, NULL);
}


// fork/exec/wait: use code from homework.
static int do_unix_cmd(char* argv[], int nargs) {
    int pid;

    safe_sys(pid = fork());

    if (pid == 0) {
        argv[nargs] = NULL;
        if (execvp(argv[0], argv) < 0) {
            printf("Unix Command Failed: %s\n", strerror(errno));
        }
        exit(0);
    }

    int status;
    waitpid(pid, &status, 0);

    if (WEXITSTATUS(status) != 0) {
        printf("Child exited with exit code %d\n", WEXITSTATUS(status));
    }

    return 0;
}

void reboot_pi(int pi_fd) {
    pi_put(pi_fd, "reboot\n");
    printf("Pi Rebooted. Shell done.\n");
    exit(0);
}

// ship pi program to the pi.
static int run_pi_prog(int pi_fd, char* argv[], int nargs) {
    if (is_pi_prog(argv[0])) {
        printf("Found pi program! %s\n", argv[0]);
        if(access(argv[0], F_OK) < 0) {
            printf("Error: Couldn't open binary.\n");
            return 0;
        }
        pi_put(pi_fd, "boot\n");
        int failed = send_program(pi_fd, argv[0]);
        switch (failed) {
            case 0: // worked
                return 1;
            case BAD_VERSION:
                printf("Error: The Pi didn't recognize the linker version of that binary.\n");
                return 2;
            case BAD_START:
                printf("Error: The Pi couldn't link that binary (the start is too low).\n");
                return 2;
            case BAD_END:
                printf("Error: The Pi couldn't link that binary (the end is too high).\n");
                return 2;
            default:
                printf("Error: There was corruption during transmission. Please try again. Code: %d\n", failed);
                return 2;
        }
    }
    return 0;
}

// run a builtin: reboot, echo, cd
static int do_builtin_cmd(int pi_fd, char* argv[], int nargs) {
    if (strncmp(argv[0], "reboot", 6) == 0) {
        printf("Unix: Found reboot. \n");
        reboot_pi(pi_fd);
        return 1;
    } else if (strncmp(argv[0], "echo", 4) == 0) {
        printf("Unix: Found echo.\n");
        for (int i = 0; i < nargs; i++) {
            pi_put(pi_fd, argv[i]);
            if (i != (nargs - 1))
                pi_put(pi_fd, " ");
        }
        pi_put(pi_fd, "\n");
        return 1;
    }
    return 0;
}

/*
 * suggested steps:
 * 	1. just do echo.
 *	2. add reboot()
 *	3. add catching control-C, with reboot.
 *	4. run simple program: anything that ends in ".bin"
 *
 * NOTE: any command you send to pi must end in `\n` given that it reads
 * until newlines!
 */
static int shell(int pi_fd, int unix_fd) {
    const unsigned maxargs = 32;
    char* argv[maxargs];
    char buf[8192];

    char in_buf[8192];

    catch_control_c();

    // wait for the welcome message from the pi?  note: we
    // will hang if the pi does not send an entire line.  not
    // sure about this: should we keep reading til newline?
    fprintf(stderr, "PIX:> ");
    while (!done && fgets(buf, sizeof buf, stdin)) {
        int n = strlen(buf) - 1;
        buf[n] = 0;

        int nargs = tokenize(argv, maxargs, buf);
        // empty line: skip.
        if (nargs == 0);
            // is it a builtin?  do it.
        else if (do_builtin_cmd(pi_fd, argv, nargs)) {
            if (pi_readline(pi_fd, in_buf, sizeof(in_buf))) {
                fprintf(stderr, "Pi Echoed: %s\n", in_buf);
            }
        }
            // if not a pi program (end in .bin) fork-exec
        else if (run_pi_prog(pi_fd, argv, nargs)) {
            echo_until(pi_fd, "PIX:> ");
            continue; // pi echoed the shell, so don't print it.
        } else {
            do_unix_cmd(argv, nargs);
        }

        fprintf(stderr, "PIX:> ");
    }

    if (done) {
        printf("\ngot control-c: going to shutdown pi.\n");
        reboot_pi(pi_fd);
    }
    return 0;
}

int main(void) {
    return shell(TRACE_FD_HANDOFF, 0);
}
