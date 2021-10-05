#ifndef C_SHELL_UTILS_H
#define C_SHELL_UTILS_H

#include <stdbool.h>
#include "jobVector.h"
#include "charVector.h"

#define MAX_PATH_LEN 2048
extern char *HOME;
extern char *CUR_DIR;
extern char *PREV_DIR;
extern vectorJob CHILD_PROC;
extern char *inputBuffer;
extern const int inputBufSize;

// Color macros
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define PINK "\x1B[35m"
#define CYAN "\x1B[36m"
#define WHITE "\x1B[37m"
#define RESET "\x1B[0m"

void cprintf(const char *color, const char *format, ...);

void initShell();

vectorString tokenize(char *, char []);

bool isPrefix(char *, char *);

void exitShell(int);

#endif
