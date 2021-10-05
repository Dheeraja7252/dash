#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "history.h"
#include "utils.h"

const char HISTORY_FILE[] = "~/shellHistory";
const int MAX_HISTORY = 20;
const int DEFAULT_HISTORY = 10;
vectorString HISTORY;

char *historyFilePath;

void initHistory() {
    HISTORY = init_vectorString();

    historyFilePath = (char *) malloc(MAX_PATH_LEN * sizeof(char));
    if(historyFilePath == NULL) {
        perror("malloc");
        return;
    }
    strcpy(historyFilePath, HOME);
    strcat(historyFilePath, HISTORY_FILE + 1);

    FILE *fd = fopen(historyFilePath, "r");
    if(fd == NULL) {
        fd = fopen(historyFilePath, "w");
        fclose(fd);
        return;
    }

    char *buf = (char *) malloc(inputBufSize);
    size_t len = inputBufSize;
    while(getline(&buf, &len, fd) != -1) {
        pushbackString(&HISTORY, buf);
        buf = (char *) malloc(inputBufSize);
    }
    free(buf);
    fclose(fd);
}

void freeHistory() {
    free(historyFilePath);
    for(int i = 0; i < HISTORY.size; i++)
        free(HISTORY.vector[i]);
    free_vectorString(HISTORY);
}

void writeHistory() {
    FILE *fd = fopen(historyFilePath, "w");
    if(fd == NULL)
        return;

    int begin = HISTORY.size - MAX_HISTORY;
    if(begin < 0)
        begin = 0;

    for(int i = begin; i < HISTORY.size; i++) {
        fprintf(fd, "%s", HISTORY.vector[i]);
    }
    fclose(fd);
}

// call after taking input
void appendHistory(char *input) {
    char *command = (char *) malloc(inputBufSize);
    strcpy(command, input);
    pushbackString(&HISTORY, command);

    if(HISTORY.size >= 2 * MAX_HISTORY) {
        for(int i = 0; i < MAX_HISTORY; i++) {
            free(HISTORY.vector[i]);
            HISTORY.vector[i] = HISTORY.vector[i + MAX_HISTORY];
        }
        HISTORY.vector = realloc(HISTORY.vector, MAX_HISTORY * sizeof(char *));
        HISTORY.size = MAX_HISTORY;
        HISTORY.capacity = MAX_HISTORY;
    }
    writeHistory();
}

void displayHistory(int n) {
    int begin = HISTORY.size - n;
    if(begin < 0)
        begin = 0;
    for(int i = begin; i < HISTORY.size; i++) {
        printf("%s", HISTORY.vector[i]);
    }
}

void history(char *argv[], int argc) {
    if(argc > 2) {
        printf("history: too many arguments\n");
        return;
    }

    if(argc == 1) {
        displayHistory(DEFAULT_HISTORY);
        return;
    }

    char *endptr = NULL;
    int hisCount = (int) strtol(argv[1], &endptr, 10);
    if(hisCount < 0 || endptr == argv[1]) {
        printf("history: 2nd argument must be a non-negative integer\n");
        return;
    }
    if(hisCount > MAX_HISTORY) {
        printf("history: max %d items can be viewed\n", MAX_HISTORY);
        hisCount = MAX_HISTORY;
    }
    displayHistory(hisCount);
}
