#include <string.h>
#include <stdio.h>
#include "parse.h"
#include "charVector.h"
#include "utils.h"
#include "echo.h"
#include "cd.h"
#include "ls.h"
#include "repeat.h"
#include "pinfo.h"
#include "systemCommands.h"
#include "history.h"

vectorString tokenize(char *str, char delim[]) {
    char *savePtr = NULL;
    char *token;
    vectorString vec = init_vectorString();
    token = strtok_r(str, delim, &savePtr);
    while(token != NULL) {
        pushbackString(&vec, token);
        token = strtok_r(NULL, delim, &savePtr);
    }
    return vec;
}

void parseInput(char *inp) {
//    printf("Parsing input: %s", inp);
//    char *savePtr = NULL;
//    char *command;
//    command = strtok_r(inp, ";", &savePtr);
//    while(command != NULL) {
//        parseCommand(command);
//        command = strtok_r(NULL, ";", &savePtr);
//    }

    vectorString vec = tokenize(inp, ";");
    for(int i = 0; i < vec.size; i++) {
        parseCommand(vec.vector[i]);
    }
    free_vectorString(vec);
}

void parseCommand(char *command) {
//    printf("Parsing command: %s", command);
//    char *savePtr = NULL;
//    char *token;
//    vectorString vec = init_vectorString();
//    token = strtok_r(command, " \n\t", &savePtr);
//    while(token != NULL) {
//        pushbackString(&vec, token);
//        token = strtok_r(NULL, " \n\t", &savePtr);
//    }

    vectorString vec = tokenize(command, " \n\t");
    pushbackString(&vec, (char *) NULL);
    execute(vec.vector, vec.size - 1);
    free_vectorString(vec);
}

void execute(char *argv[], int argc) {
    if(argc == 0) return;

    if(strcmp(argv[0], "cd") == 0) {
        cd(argv, argc);
        return;
    }
    if(strcmp(argv[0], "echo") == 0) {
        echo(argv, argc);
        return;
    }
    if(strcmp(argv[0], "pwd") == 0) {
        printf("%s\n", CUR_DIR);
        return;
    }
    if(strcmp(argv[0], "ls") == 0) {
        ls(argv, argc);
        return;
    }
    if(strcmp(argv[0], "repeat") == 0) {
        repeat(argv, argc);
        return;
    }
    if(strcmp(argv[0], "pinfo") == 0) {
        pinfo(argv, argc);
        return;
    }
    if(strcmp(argv[0], "history") == 0) {
        history(argv, argc);
        return;
    }
    if(strcmp(argv[0], "exit") == 0) {
        exitShell(0);
        return;
    }
    //
    systemCommand(argv, argc);
}
