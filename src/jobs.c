#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "jobVector.h"
#include "jobs.h"
#include "utils.h"

int cmp(const void *a, const void *b) {
    struct job *j1 = (struct job *) a;
    struct job *j2 = (struct job *) b;

    int cmp = strcmp(j1->pname, j2->pname);
    if (cmp == 0)
        return (j1->jobNum - j2->jobNum);
    return cmp;
}

void jobs(char *argv[], int argc) {
    int flagr = 0, flags = 0;

    if (argc > 2) {
        fprintf(stderr, "jobs: too many arguments\n");
        return;
    }
    if (argc == 1)
        flagr = flags = 1;
    else if (strcmp(argv[1], "-r") == 0)
        flagr = 1;
    else if (strcmp(argv[1], "-s") == 0)
        flags = 1;
    else {
        fprintf(stderr, "jobs: invalid arguments\n");
        return;
    }

    // put jobs in vec, sort, go over vector with flags, open stat print f needed.
    vectorJob curJobs = init_vectorJob();
    for (int i = 0; i < CHILD_PROC.size; i++) {
        pushbackJob(&curJobs, CHILD_PROC.vector[i].pid, CHILD_PROC.vector[i].pname);
    }
    qsort(curJobs.vector, curJobs.size, sizeof(struct job), cmp);

    for (int i = 0; i < curJobs.size; i++) {
        char filename[100];
        int pid = curJobs.vector[i].pid, job = curJobs.vector[i].jobNum;
        sprintf(filename, "/proc/%d/stat", pid);
        FILE *fd = fopen(filename, "r");
        if (fd == NULL) {
            perror("pinfo");
            return;
        }
        char *lineptr = NULL;
        size_t len = 0;
        if (getline(&lineptr, &len, fd) < 0 || lineptr == NULL) {
            printf("pinfo: error retrieving process info of process with pid %d\n", pid);
            return;
        }
        vectorString tokens = tokenize(lineptr, " \n");

        if (flagr == 1 && (tokens.vector[2][0] == 'R' || tokens.vector[2][0] == 'S')) {
            printf("[%d] Running %s [%d]\n", job, curJobs.vector[i].pname, pid);
        }
        else if (flags == 1 && tokens.vector[2][0] == 'T') {
            printf("[%d] Stopped %s [%d]\n", job, curJobs.vector[i].pname, pid);
        }

        free_vectorString(tokens);
    }

    free_vectorJob(curJobs);
}

void sig(char *argv[], int argc) {
    if (argc > 3) {
        fprintf(stderr, "sig: too many arguments\n");
        return;
    }

    int jobNum, signal;
    char *endptr = NULL;
    jobNum = (int) strtol(argv[1], &endptr, 10);
    if (jobNum <= 0 || endptr == argv[1]) {
        fprintf(stderr, "sig: 2nd argument must be a positive integer\n");
        return;
    }

    endptr = NULL;
    signal = (int) strtol(argv[2], &endptr, 10);
    if (signal < 0 || endptr == argv[2]) {
        fprintf(stderr, "sig: 3rd argument must be a non-negative integer\n");
        return;
    }

    if (jobNum > CHILD_PROC.size) {
        fprintf(stderr, "sig: invalid job number\n");
        return;
    }

    int pid = CHILD_PROC.vector[jobNum - 1].pid;
    if (kill(pid, signal) == -1) {
        fprintf(stderr, "sig: invalid signal\n");
    }
}

void bg(char *argv[], int argc) {
    if (argc > 2) {
        fprintf(stderr, "bg: too many arguments\n");
        return;
    }
    if (argc == 1) {
        fprintf(stderr, "bg: too few arguments\n");
        return;
    }

    int jobNum;
    char *endptr = NULL;
    jobNum = (int) strtol(argv[1], &endptr, 10);
    if (jobNum <= 0 || endptr == argv[1]) {
        fprintf(stderr, "bg: 2nd argument must be a positive integer\n");
        return;
    }

    if (jobNum > CHILD_PROC.size) {
        fprintf(stderr, "sig: invalid job number\n");
        return;
    }

    int pid = CHILD_PROC.vector[jobNum - 1].pid;
    if (kill(pid, SIGCONT) == -1) {
        perror("bg");
        return;
    }
}

void fg(char *argv[], int argc) {
    if (argc > 2) {
        fprintf(stderr, "fg: too many arguments\n");
        return;
    }
    if (argc == 1) {
        fprintf(stderr, "fg: too few arguments\n");
        return;
    }

    int jobNum;
    char *endptr = NULL;
    jobNum = (int) strtol(argv[1], &endptr, 10);
    if (jobNum <= 0 || endptr == argv[1]) {
        fprintf(stderr, "fg: 2nd argument must be a positive integer\n");
        return;
    }

    if (jobNum > CHILD_PROC.size) {
        fprintf(stderr, "fg: invalid job number\n");
        return;
    }

    int pid = CHILD_PROC.vector[jobNum - 1].pid;
    if (kill(pid, SIGCONT) == -1) {
        perror("fg");
        return;
    }

    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    tcsetpgrp(STDIN_FILENO, pid);
    int status;
    waitpid(pid, &status, WUNTRACED);
    if (!WIFSTOPPED(status))
        removeJob(&CHILD_PROC, pid);
    tcsetpgrp(STDIN_FILENO, getpgrp());
    signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
}
