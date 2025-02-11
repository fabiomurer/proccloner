#include "load_linux.h"

#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/personality.h>

void error_and_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

pid_t load_linux(char** argv, struct user_regs_struct* saved_regs) {
    int status = 0;
    pid_t child = fork();

    if (child == -1) {
        error_and_exit("fork");
    }

    if (child == 0) {
        // Child process: request tracing and stop itself so the parent can attach.
        if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) {
            error_and_exit("ptrace(PTRACE_TRACEME)");
        }
        // Stop so the parent can set options.
        raise(SIGSTOP);
        
        // turn off address randomization
        if (personality(ADDR_NO_RANDOMIZE) == -1) {
            error_and_exit("personality(ADDR_NO_RANDOMIZE)");
        }

        // Replace the child process with the target program.
        // Note: We pass argv[0] as the program and &argv[0] as its arguments.
        if (execvp(argv[0], &argv[0]) == -1) {
            fprintf(stderr, "execvp failed: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process.
        // Wait for child to stop (due to SIGSTOP from the child).
        if (waitpid(child, &status, 0) == -1) {
            error_and_exit("waitpid");
        }

        if (!WIFSTOPPED(status)) {
            fprintf(stderr, "Child did not stop as expected.\n");
            exit(EXIT_FAILURE);
        }
        
        // Set options to catch exec event.
        if (ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACEEXEC) == -1) {
            error_and_exit("ptrace(PTRACE_SETOPTIONS)");
        }

        // Resume the child process. It will now execute until the exec call.
        if (ptrace(PTRACE_CONT, child, 0, 0) == -1) {
            error_and_exit("ptrace(PTRACE_CONT)");
        }

        // Wait for the child to hit the exec event.
        if (waitpid(child, &status, 0) == -1) {
            error_and_exit("waitpid");
        }

        // Check if this stop is due to the exec event.
        if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP && (status >> 16) == PTRACE_EVENT_EXEC) {
            printf("process with pid: %d is loaded and stopped\n", child);

            
            if (ptrace(PTRACE_GETREGS, child, NULL, saved_regs) == -1) {
                error_and_exit("ptrace(PTRACE_GETREGS)");
            }

            printf("REGS:\n");
            printf("\tRIP: 0x%llx\n", saved_regs->rip);
            printf("\tRSP: 0x%llx\n", saved_regs->rsp);
            printf("\tRBP: 0x%llx\n", saved_regs->rbp);
            printf("\tRAX: 0x%llx\n", saved_regs->rax);
            printf("\tRBX: 0x%llx\n", saved_regs->rbx);
            printf("\tRCX: 0x%llx\n", saved_regs->rcx);

            return child;
        } else {
            fprintf(stderr, "Unexpected stop before exec event occurred.\n");
            exit(EXIT_FAILURE);
        }
    }
    // not reachable
    return -1;
}