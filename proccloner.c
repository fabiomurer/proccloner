#include "reader.h"
#include "load_linux.h"
#include "execute.h"

#include <linux/limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <program> [args ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t program_pid = load_linux(&argv[1]);

    read_mapping(program_pid);

    if (kill(program_pid, SIGKILL) == -1) {
        perror("kill");
        exit(EXIT_FAILURE);
    }

    int status;
    waitpid(program_pid, &status, 0);

    transfer_execution();
    
    return 0;
}