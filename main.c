#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "src/prompt.h"
#include "src/utils.h"
#include "src/parse.h"
#include "src/history.h"

int main() {
    initShell();
    size_t bufLen = 0;
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    while (1) {
        displayPrompt();
        if (getline(&inputBuffer, &bufLen, stdin) == EOF) {
            break;
        }
        if (inputBuffer == NULL) continue;
        appendHistory(inputBuffer);
        parseInput(inputBuffer);
        free(inputBuffer);
        inputBuffer = NULL;
        bufLen = 0;
    }
    exitShell(0);
    return 0;
}
