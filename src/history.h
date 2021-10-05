#ifndef C_SHELL_HISTORY_H
#define C_SHELL_HISTORY_H

#include "charVector.h"

extern const char HISTORY_FILE[];
extern const int MAX_HISTORY;
extern const int DEFAULT_HISTORY;
extern vectorString HISTORY;

void initHistory();

void appendHistory(char *);

void history(char **, int);

void freeHistory();

#endif
