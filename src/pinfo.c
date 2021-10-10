#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "pinfo.h"
#include "utils.h"
#include "charVector.h"
#include "prompt.h"

void display_pinfo(int);

void pinfo(char *argv[], int argc) {
    if (argc > 2) {
        printf("pinfo: too many arguments\n");
        return;
    }

    if (argc == 2) {
        char *endptr = NULL;
        int pid = (int) strtol(argv[1], &endptr, 10);
        if (pid < 0 || endptr == argv[1]) {
            printf("pinfo: invalid pid\n");
            return;
        }
        display_pinfo(pid);
        return;
    }

    display_pinfo(getpid());
}

void display_pinfo(int pid) {
    char filename[100];
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

    sprintf(filename, "/proc/%d/exe", pid);
    char exePath[MAX_PATH_LEN];
    int pathLen;
    if ((pathLen = (int) readlink(filename, exePath, MAX_PATH_LEN - 1)) < 0) {
        printf("pinfo: error retrieving process info of process with pid %d\n", pid);
        return;
    }
    exePath[pathLen] = '\0';

    char procStatus[5];
    strcpy(procStatus, tokens.vector[2]);
    if (strcmp(tokens.vector[4], tokens.vector[7]) == 0)
        strcat(procStatus, "+");

    printf("pid - %d\n", pid);
    printf("Process Status - %s\n", procStatus);
    printf("Memory - %s\n", tokens.vector[22]);
    if (isPrefix(HOME, exePath))
        printf("Executable Path - ~%s\n", exePath + strlen(HOME));
    else
        printf("Executable Path - %s\n", exePath);

    free_vectorString(tokens);
}
