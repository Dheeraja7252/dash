#include <pwd.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "prompt.h"
#include "utils.h"

#define hostNameSize 1024

void displayPrompt() {
    char *username = getpwuid(getuid())->pw_name;
    char hostname[hostNameSize];
    hostname[hostNameSize - 1] = '\0';
    gethostname(hostname, hostNameSize - 1);

    if(isPrefix(HOME, CUR_DIR)) {
//        printf("<%s@%s:~%s> ", username, hostname, CUR_DIR + strlen(HOME));
        printf("<");
        cprintf(GREEN, "%s@%s", username, hostname);
        printf(":");
        cprintf(BLUE, "~%s", CUR_DIR + strlen(HOME));
        printf("> ");
    }
    else {
//        printf("<%s@%s:%s> ", username, hostname, CUR_DIR);
        printf("<");
        cprintf(GREEN, "%s@%s", username, hostname);
        printf(":");
        cprintf(BLUE, "%s", CUR_DIR);
        printf("> ");
    }
}


