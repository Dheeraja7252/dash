#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "cd.h"
#include "utils.h"

void cdError(int err);

void cd(char *argv[], int argc) {
    if(argc > 2) {
        printf("cd: too many arguments\n");
        return;
    }
    // no arguments, cd to home
    if(argc == 1) {
        int status = chdir(HOME);
        if(status < 0) {
            cdError(errno);
            return;
        }
        strcpy(PREV_DIR, CUR_DIR);
        getcwd(CUR_DIR, MAX_PATH_LEN);
        return;
    }

    // cd to prev directory
    if(strcmp(argv[1], "-") == 0) {
        int status = chdir(PREV_DIR);
        if(status < 0) {
            cdError(errno);
            return;
        }
        strcpy(PREV_DIR, CUR_DIR);
        getcwd(CUR_DIR, MAX_PATH_LEN);
        return;
    }


    // relative path from home
    if(argv[1][0] == '~') {
        char absolutePath[MAX_PATH_LEN];
        strcpy(absolutePath, HOME);
        strcat(absolutePath, argv[1] + 1);

        int status = chdir(absolutePath);
        if(status < 0) {
            cdError(errno);
            return;
        }
        strcpy(PREV_DIR, CUR_DIR);
        getcwd(CUR_DIR, MAX_PATH_LEN);
        return;
    }
    // absolute path or relative path from cur dir
    int status = chdir(argv[1]);
    if(status < 0) {
        cdError(errno);
        return;
    }
    strcpy(PREV_DIR, CUR_DIR);
    getcwd(CUR_DIR, MAX_PATH_LEN);
}

void cdError(int err) {
    perror("cd");
}
