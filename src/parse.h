#ifndef C_SHELL_PARSE_H
#define C_SHELL_PARSE_H

void parseInput(char *inp);

void parseCommand(char *);

void parsePipeline(char **, int);

int parseRedir(char **, int);

void execute(char **, int);

#endif
