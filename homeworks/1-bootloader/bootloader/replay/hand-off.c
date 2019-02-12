// simplest example of handing off and running process: open the tty, dup
// the fd, fork/exec my-install, wait and print exit code.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>

#include "trace.h"
#include "support.h"
#include "demand.h"

// synchronously wait for <pid> to exit.  Return its exit code.
static int exit_code(int pid) {
    int status;
    safe_sys(waitpid(pid, &status, 0));
    if (!WIFEXITED(status)) {
        return -1; // did not exit normally
    }
    return WEXITSTATUS(status);
}

// run:
// 	1. fork
// 	2. in child:
// 		a. remap <fd> to <TRACE_FD_HANDOFF>
//		b. execvp the process in <argv[]>
//	3. in parent: 
//		a. wait for child.
//		b. print its exit code or if it crashed.
//
//  Note: that when you run my-install with tracing, the output
//  should be identical to running it raw.
void run(int fd, char* argv[]) {
    int pid = 0;

    safe_sys(pid = fork());

    if (pid == 0) {
        safe_sys(dup2(fd, TRACE_FD_HANDOFF));
        safe_sys(close(fd));
        safe_sys(execvp(argv[0], argv));

        return;
    }

    fprintf(stderr, "child %d: exited with: %d\n", pid, exit_code(pid));
}

int main(int argc, char* argv[]) {
    // open tty dev, set it to be 8n1  and 115200 baud
    const char* portname = 0;
    int fd = open_tty(&portname);
    fd = set_tty_to_8n1(fd, B115200, 1);

    run(fd, &argv[1]);
    return 0;
}
