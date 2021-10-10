#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <stdarg.h>
#include "utils.h"
#include "history.h"
#include "systemCommands.h"

char *HOME;
char *CUR_DIR;
char *PREV_DIR;
vectorJob CHILD_PROC;
char *inputBuffer;
const int inputBufSize = 1024;

void initShell() {
    HOME = malloc(MAX_PATH_LEN * sizeof(char));
    CUR_DIR = malloc(MAX_PATH_LEN * sizeof(char));
    PREV_DIR = malloc(MAX_PATH_LEN * sizeof(char));

    if (HOME == NULL || CUR_DIR == NULL || PREV_DIR == NULL) {
        perror("malloc");
        exit(1);
    }
    getcwd(HOME, MAX_PATH_LEN - 1);
    getcwd(CUR_DIR, MAX_PATH_LEN - 1);
    getcwd(PREV_DIR, MAX_PATH_LEN - 1);

    CHILD_PROC = init_vectorJob();
    signal(SIGCHLD, childHandler);

    initHistory();

    inputBuffer = NULL;
}

// checks if the path s1 is prefix of s2
bool isPrefix(char *s1, char *s2) {
    int len1 = strlen(s1), len2 = strlen(s2);
    if (len1 > len2) return false;
    for (int i = 0; i < len1; i++) {
        if (s1[i] != s2[i])
            return false;
    }
    if (len1 == len2 || s2[len1] == '/' || s2[len1] == '\0')
        return true;
    return false;
}

// TODO: handle children before exit
void exitShell(int st) {
    free(HOME);
    free(CUR_DIR);
    free(PREV_DIR);
    free_vectorJob(CHILD_PROC);
    freeHistory();
    free(inputBuffer);
    if (st == 0)
        printf("Bye :)\n\n");
    exit(0);
}

void cprintf(const char *color, const char *format, ...) {
    printf("%s", color);

    va_list argptr;
    va_start(argptr, format);
    vprintf(format, argptr);
    va_end(argptr);

    printf("%s", RESET);
}
