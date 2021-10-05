#include <stdlib.h>
#include <stdio.h>
#include "jobVector.h"

vectorJob init_vectorJob() {
    vectorJob vec;
    vec.size = 0;
    vec.capacity = 10;
    vec.vector = (struct job *) malloc(10 * sizeof(struct job));
    if(vec.vector == NULL) {
        perror("malloc");
        exit(1);
    }
    return vec;
}

void pushbackJob(vectorJob *vec, int pid, char *pname) {
    if(vec->size == vec->capacity) {
        vec->vector = realloc(vec->vector, 2 * vec->capacity * (sizeof(struct job)));
        vec->capacity = 2 * vec->capacity;
    }
    vec->vector[vec->size].pid = pid;
    vec->vector[vec->size].pname = pname;
    vec->size++;
}

char *jobName(vectorJob vec, int pid) {
    for(int i = 0; i < vec.size; i++) {
        if(vec.vector[i].pid == pid)
            return vec.vector[i].pname;
    }
    return NULL;
}

void removeJob(vectorJob *vec, int pid) {
    for(int i = 0; i < vec->size - 1; i++) {
        if(vec->vector[i].pid == pid) {
            free(vec->vector[i].pname);
            vec->vector[i] = vec->vector[vec->size - 1];
            vec->size--;
            return;
        }
    }
    if(vec->vector[vec->size - 1].pid == pid) {
        free(vec->vector[vec->size - 1].pname);
        vec->size--;
        return;
    }
}

void free_vectorJob(vectorJob vec) {
    for(int i = 0; i < vec.size; i++)
        free(vec.vector[i].pname);
    free(vec.vector);
}
