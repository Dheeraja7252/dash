#ifndef C_SHELL_CHARVECTOR_H
#define C_SHELL_CHARVECTOR_H

typedef struct char_vector {
    char **vector;
    int size;
    int capacity;
} vectorString;

vectorString init_vectorString();

void pushbackString(vectorString *, char *);

void free_vectorString(vectorString);

#endif
