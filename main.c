#include <stdio.h>
#include <unistd.h>
#include "src/prompt.h"
#include "src/utils.h"
#include "src/parse.h"
#include "src/history.h"

int main() {
    initShell();
    size_t bufLen = inputBufSize;
    while(1) {
        displayPrompt();
        getline(&inputBuffer, &bufLen, stdin);
        appendHistory(inputBuffer);
        parseInput(inputBuffer);
    }
    return 0;
}
