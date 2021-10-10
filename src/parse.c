#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
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
#include "jobs.h"

void resetIO(int saveInFD, int saveOutFD) {
    dup2(saveInFD, STDIN_FILENO);
    dup2(saveOutFD, STDOUT_FILENO);
    close(saveInFD);
    close(saveOutFD);
}

vectorString tokenize(char *str, char delim[]) {
    char *savePtr = NULL;
    char *token;
    vectorString vec = init_vectorString();
    token = strtok_r(str, delim, &savePtr);
    while (token != NULL) {
        pushbackString(&vec, token);
        token = strtok_r(NULL, delim, &savePtr);
    }
    return vec;
}

void parseInput(char *inp) {
    vectorString vec = tokenize(inp, ";");
    for (int i = 0; i < vec.size; i++) {
        parseCommand(vec.vector[i]);
    }
    free_vectorString(vec);
}

void parseCommand(char *command) {
    vectorString vec = tokenize(command, " \n\t");
    //    pushbackString(&vec, (char *) NULL);
    //    execute(vec.vector, vec.size - 1);
    if (vec.size == 0) {
        free_vectorString(vec);
        return;
    }
    parsePipeline(vec.vector, vec.size);
    free_vectorString(vec);
}

void parsePipeline(char *argv[], int argc) {
    int countPipes = 0;
    for (int i = 0; i < argc; i++) {
        if (strcmp("|", argv[i]) == 0)
            countPipes++;
    }
    if (strcmp(argv[argc - 1], "|") == 0) {
        printf("shell: pipe syntax error\n");
        return;
    }
    if (countPipes == 0) {
        // no pipelines
        parseRedir(argv, argc);
        return;
    }
    int saveInFD = dup(STDIN_FILENO);
    int saveOutFD = dup(STDOUT_FILENO);

    int rd = 0, wr = 1;
    int pipeline[2];

    int ind = 0, commandsLeft = countPipes + 1;
    while (ind < argc) {
        vectorString args = init_vectorString();
        while (ind < argc && strcmp(argv[ind], "|") != 0) {
            pushbackString(&args, argv[ind]);
            ind++;
        }
        ind++;

        if (commandsLeft == 1) { // last command, output to terminal
            dup2(saveOutFD, STDOUT_FILENO);
        }
        else {
            if (pipe(pipeline) < 0) {
                fprintf(stderr, "pipe: error creating pipeline\n");
                resetIO(saveInFD, saveOutFD);
                return;
            }
            dup2(pipeline[wr], STDOUT_FILENO);
            close(pipeline[wr]);
        }
        parseRedir(args.vector, args.size);
        commandsLeft--;

        if (commandsLeft > 0) {
            dup2(pipeline[rd], STDIN_FILENO);
            close(pipeline[rd]);
        }
        free_vectorString(args);
    }

    resetIO(saveInFD, saveOutFD);
}

// TODO:
//  handle relative file path
//  add return codes
//  print error to stderr

void parseRedir(char *argv[], int argc) {
    if (argc == 0) return;

    vectorString vec = init_vectorString();
    int indIn = -1, indOut = -1;
    for (int i = 0; i < argc; i++) {
        if (strcmp("<", argv[i]) == 0) {
            if (indIn != -1) {
                fprintf(stderr, "Error: multiple input redirections\n");
                return;
            }
            indIn = i;
            i++; // next arg is file name
        }
        else if (strcmp(">", argv[i]) == 0 || strcmp(">>", argv[i]) == 0) {
            if (indOut != -1) {
                fprintf(stderr, "Error: multiple input redirections\n");
                return;
            }
            indOut = i;
            i++; // next arg is file name
        }
        else
            pushbackString(&vec, argv[i]);
    }
    int saveInFD = dup(STDIN_FILENO);
    int saveOutFD = dup(STDOUT_FILENO);

    if (indIn != -1) {
        if (indIn == argc - 1) {
            fprintf(stderr, "Error: expected token after >\n");
            resetIO(saveInFD, saveOutFD);
            return;
        }
        int fd = open(argv[indIn + 1], O_RDONLY);
        if (fd == -1) {
            perror("error opening input file");
            resetIO(saveInFD, saveOutFD);
            return;
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    if (indOut != -1) {
        if (indOut == argc - 1) {
            fprintf(stderr, "Error: expected token after %s \n", argv[indOut]);
            resetIO(saveInFD, saveOutFD);
            return;
        }
        int fd;
        if (strcmp(">", argv[indOut]) == 0)
            fd = open(argv[indOut + 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
        else
            fd = open(argv[indOut + 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
        if (fd == -1) {
            perror("error opening output file");
            resetIO(saveInFD, saveOutFD);
            return;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    pushbackString(&vec, NULL);
    execute(vec.vector, vec.size - 1);

    resetIO(saveInFD, saveOutFD);
}

void execute(char *argv[], int argc) {
    if (argc == 0) return;

    if (strcmp(argv[0], "cd") == 0) {
        cd(argv, argc);
        return;
    }
    if (strcmp(argv[0], "echo") == 0) {
        echo(argv, argc);
        return;
    }
    if (strcmp(argv[0], "pwd") == 0) {
        printf("%s\n", CUR_DIR);
        return;
    }
    if (strcmp(argv[0], "ls") == 0) {
        ls(argv, argc);
        return;
    }
    if (strcmp(argv[0], "repeat") == 0) {
        repeat(argv, argc);
        return;
    }
    if (strcmp(argv[0], "pinfo") == 0) {
        pinfo(argv, argc);
        return;
    }
    if (strcmp(argv[0], "history") == 0) {
        history(argv, argc);
        return;
    }
    if (strcmp(argv[0], "jobs") == 0) {
        jobs(argv, argc);
        return;
    }
    if (strcmp(argv[0], "sig") == 0) {
        sig(argv, argc);
        return;
    }
    if (strcmp(argv[0], "bg") == 0) {
        bg(argv, argc);
        return;
    }
    if (strcmp(argv[0], "fg") == 0) {
        fg(argv, argc);
        return;
    }
    if (strcmp(argv[0], "exit") == 0) {
        exitShell(0);
        return;
    }
    systemCommand(argv, argc);
}
