#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "systemCommands.h"
#include "utils.h"
#include "prompt.h"

void foregroundProc(char **argv, int argc);

void backgroundProc(char **argv, int argc);

void systemCommand(char **argv, int argc) {
    if(strcmp(argv[argc - 1], "&") == 0) {
        argv[argc - 1] = NULL;
        backgroundProc(argv, argc - 1);
        return;
    }
    else {
        foregroundProc(argv, argc);
        return;
    }
}

void backgroundProc(char **argv, int argc) {
    int pid = fork();

    if(pid < 0) {
        printf("Error invoking command\n");
        return;
    }
    if(pid == 0) {
        if(execvp(argv[0], argv) < 0) {
            printf("%s: command not found\n", argv[0]);
            exitShell(1);
            // exit
        }
        return;
    }
    if(pid > 0) {
        printf("%d\n", pid);
        pushbackJob(&CHILD_PROC, pid, argv[0]);
        return;
    }
}

void foregroundProc(char **argv, int argc) {
    int pid = fork();

    if(pid < 0) {
        printf("Error invoking command\n");
        return;
    }
    if(pid == 0) {
        if(execvp(argv[0], argv) < 0) {
            printf("%s: command not found\n", argv[0]);
            exit(0);
        }
        return;
    }
    if(pid > 0) {
        waitpid(pid, NULL, 0);
        return;
    }
}

void childHandler(int signalNum) {
    int status;
    pid_t pid;
    while((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) {
        char *name = jobName(CHILD_PROC, pid);
        if(name == NULL)
            continue;

        if(WIFEXITED(status)) {
            printf("\n%s with pid %d exited normally\n", name, pid);
            removeJob(&CHILD_PROC, pid);
        }
        else if(WIFSTOPPED(status)) {
            printf("%s with pid %d suspended normally\n", name, pid);
            removeJob(&CHILD_PROC, pid);
        }
        else {
            printf("%s with pid %d did not exit normally\n", name, pid);
            removeJob(&CHILD_PROC, pid);
        }
        displayPrompt();
        fflush(stdout);
    }
}
