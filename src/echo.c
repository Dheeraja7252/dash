#include <stdio.h>
#include "echo.h"

void echo(char *argv[], int argc) {
    for(int i = 1; i < argc; i++)
        printf("%s ", argv[i]);
    printf("\n");
}
