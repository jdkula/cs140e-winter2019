#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "redirect.h"
#include "demand.h"

// redir:
//  fork/exec <pi_process>
// return two file descripts:
//  rd_fd = the file descriptor the parent reads from to see the child's
//      stdout/stderr output.
//  wr_fd = the file descriptor the parent writes to in order to write data
//      to the child's stdin.
//
// you will override the child's stdin/stdout/stderr using pipes and dup2.
//      recall: pipe[0] = read in, pipe[1] = write end.
int redir(int* rd_fd, int* wr_fd, char* const pi_process) {
    // make sure you close all the fd's you need to! 
    // if the test code hangs, this is probably the issue.

    int to_process[2];
    int from_process[2];

    safe_sys(pipe(to_process));
    safe_sys(pipe(from_process));

    *rd_fd = from_process[0];
    *wr_fd = to_process[1];

    int pid;

    safe_sys(pid = fork())

    if (pid == 0) {
        safe_sys(dup2(to_process[0], STDIN_FILENO));
        safe_sys(close(to_process[0]));
        safe_sys(close(to_process[1]));
        safe_sys(dup2(from_process[1], STDERR_FILENO));
        safe_sys(dup2(from_process[1], STDOUT_FILENO));
        safe_sys(close(from_process[0]))
        safe_sys(close(from_process[1]))

        char* argv[2];
        argv[0] = pi_process;
        argv[1] = NULL;

        safe_sys(execvp(pi_process, argv));
        exit(1);
    }

    safe_sys(close(from_process[1]));
    safe_sys(close(to_process[0]));

    return pid;
}


int fd_putc(int fd, char c) {
    if (write(fd, &c, 1) != 1)
        sys_die(write, write
                failed);
    return 0;
}

void fd_puts(int fd, const char* msg) {
    while (*msg)
        fd_putc(fd, *msg++);
}


