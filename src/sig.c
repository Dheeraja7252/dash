#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "jobVector.h"
#include "sig.h"
#include "utils.h"

void sig(char *argv[], int argc) {
    if (argc > 3) {
        fprintf(stderr, "sig: too many arguments\n");
        return;
    }

    int pid, signal;
    char *endptr = NULL;
    pid = (int) strtol(argv[1], &endptr, 10);
    if (pid < 0 || endptr == argv[1]) {
        fprintf(stderr, "sig: 2nd argument must be a non-negative integer\n");
        return;
    }

    endptr = NULL;
    signal = (int) strtol(argv[2], &endptr, 10);
    if (signal < 0 || endptr == argv[2]) {
        fprintf(stderr, "sig: 3rd argument must be a non-negative integer\n");
        return;
    }

    if (jobName(CHILD_PROC, pid) == NULL) {
        fprintf(stderr, "sig: invalid pid\n");
        return;
    }

    // conditions on signals?

    kill(pid, signal);
}
