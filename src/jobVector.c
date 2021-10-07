#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "jobVector.h"

vectorJob init_vectorJob() {
    vectorJob vec;
    vec.size = 0;
    vec.capacity = 10;
    vec.vector = (struct job *) malloc(10 * sizeof(struct job));
    if (vec.vector == NULL) {
        perror("malloc");
        exit(1);
    }
    return vec;
}

void pushbackJob(vectorJob *vec, int pid, char *pname) {
    if (vec->size == vec->capacity) {
        vec->vector = realloc(vec->vector, 2 * vec->capacity * (sizeof(struct job)));
        vec->capacity = 2 * vec->capacity;
    }
    vec->vector[vec->size].pname = (char *) malloc(strlen(pname));
    if (vec->vector[vec->size].pname == NULL) {
        perror("malloc");
        return;
    }
    strcpy(vec->vector[vec->size].pname, pname);
    vec->vector[vec->size].pid = pid;
    vec->size++;
}

char *jobName(vectorJob vec, int pid) {
    for (int i = 0; i < vec.size; i++) {
        if (vec.vector[i].pid == pid)
            return vec.vector[i].pname;
    }
    return NULL;
}

void removeJob(vectorJob *vec, int pid) {
    int jobInd = -1;
    for (int i = 0; i < vec->size; i++) {
        if (vec->vector[i].pid == pid) {
            jobInd = i;
            break;
        }
    }
    if (jobInd == -1) return;

    free(vec->vector[jobInd].pname);
    for (int i = jobInd; i + 1 < vec->size; i++) {
        vec->vector[i].pname = vec->vector[i + 1].pname;
        vec->vector[i].pid = vec->vector[i + 1].pid;
    }
    vec->vector[vec->size - 1].pname = NULL;
    vec->vector[vec->size - 1].pid = 0;
    vec->size--;
}

void free_vectorJob(vectorJob vec) {
    for (int i = 0; i < vec.size; i++)
        free(vec.vector[i].pname);
    free(vec.vector);
}
