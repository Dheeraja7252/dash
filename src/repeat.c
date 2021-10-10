#include <stdio.h>
#include <stdlib.h>
#include "repeat.h"
#include "parse.h"

void repeat(char *argv[], int argc) {
    if (argc <= 2) {
        printf("repeat: invalid arguments\n");
        return;
    }
    char *endptr = NULL;
    int repCount = (int) strtol(argv[1], &endptr, 10);
    if (repCount < 0 || endptr == argv[1]) {
        printf("repeat: 2nd argument must be a non-negative integer\n");
        return;
    }
    for (int i = 0; i < repCount; i++) {
        execute(argv + 2, argc - 2);
    }
}
