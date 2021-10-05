#include <stdlib.h>
#include <stdio.h>
#include "charVector.h"

vectorString init_vectorString() {
    vectorString vec;
    vec.size = 0;
    vec.capacity = 10;
    vec.vector = (char **) malloc(10 * sizeof(char *));
    if(vec.vector == NULL) {
        perror("malloc");
        exit(1);
    }
    return vec;
}

void pushbackString(vectorString *vec, char *str) {
    if(vec->size == vec->capacity) {
        vec->vector = realloc(vec->vector, 2 * vec->capacity * (sizeof(char *)));
        vec->capacity = 2 * vec->capacity;
    }
    vec->vector[vec->size] = str;
    vec->size++;
}

void free_vectorString(vectorString vec) {
    free(vec.vector);
}
