#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include "ls.h"
#include "charVector.h"
#include "utils.h"

int isHidden(char *name) {
    int ind = -1;
    for(int i = 0; name[i] != '\0'; i++) {
        if(name[i] == '/')
            ind = i;
    }
    if(name[ind + 1] == '.')
        return 1;
    return 0;
}

long totalBlocks(char dirName[], int flaga) {
    DIR *d = opendir(dirName);
    struct dirent *de;
    long total = 0;
    while((de = readdir(d)) != NULL) {
        if(flaga == 1 || !isHidden(de->d_name)) {
            char filePath[MAX_PATH_LEN];
            if(dirName[0] == '~')
                sprintf(filePath, "%s%s/%s", HOME, dirName + 1, de->d_name);
            else
                sprintf(filePath, "%s/%s", dirName, de->d_name);

            struct stat statBuf;
            stat(filePath, &statBuf);
            total += statBuf.st_blocks;
        }
    }
    closedir(d);
    return total / 2;
}

char *lsPerms(mode_t mode) {
    static char bits[11];
    if(S_ISDIR(mode))
        bits[0] = 'd';
    else if(S_ISLNK(mode))
        bits[0] = 'l';
    else
        bits[0] = '-';

    char ref[] = "-rwxrwxrwx";
    int bit = 1;
    for(int i = 9; i > 0; i--) {
        if((mode & bit) == bit)
            bits[i] = ref[i];
        else
            bits[i] = '-';
        bit = bit * 2;
    }
    bits[10] = '\0';
    return (bits);
}

void lsFile(char dirName[], char fileName[], int flagl, int flaga) {
    char filePath[MAX_PATH_LEN];
    if(fileName[0] == '~')
        sprintf(filePath, "%s%s", HOME, fileName + 1);
    else
        sprintf(filePath, "%s/%s", dirName, fileName);

    struct stat statBuf;
    if(stat(filePath, &statBuf) == -1) {
        perror("ls");
        return;
    }
    if(isHidden(fileName) && flaga == 0)
        return;
    if(flagl == 0) {
        printf("%s\n", fileName);
        return;
    }

    printf("%s ", lsPerms(statBuf.st_mode));
    printf("%4ld ", statBuf.st_nlink);
    printf("%s ", getpwuid(statBuf.st_uid)->pw_name);
    printf("%s ", getgrgid(statBuf.st_gid)->gr_name);
    printf("%13ld ", statBuf.st_size);

    char date[21];
    time_t curTime = time(NULL);
    if((curTime - (statBuf.st_ctime)) >= 31536000) // if file is older than a year
        strftime(date, 20, "%b %d %H:%M", localtime(&(statBuf.st_ctime)));
    else
        strftime(date, 20, "%b %d  %Y", localtime(&(statBuf.st_ctime)));
    printf("%s ", date);
    printf("%s\n", fileName);
}

void lsDir(char dir[], int flagl, int flaga) {
    char dirName[MAX_PATH_LEN];
    if(dir[0] == '~') {
        strcpy(dirName, HOME);
        strcat(dirName, dir + 1);
    }
    else
        strcpy(dirName, dir);

    DIR *d = opendir(dirName);
    if(d == NULL) {
        if(errno == ENOENT)
            printf("ls: no such file or directory\n");
        else
            printf("ls: Unable to read directory %s\n", dirName);
        return;
    }
    closedir(d);

    struct dirent *de;
    d = opendir(dirName);
    if(flagl)
        printf("total %ld\n", totalBlocks(dirName, flaga));
    while((de = readdir(d)) != NULL) {
        lsFile(dirName, de->d_name, flagl, flaga);
    }
    closedir(d);
}

void ls(char *argv[], int argc) {
    int flaga = 0, flagl = 0;
    int opt;
    optind = 0;
    while((opt = getopt(argc, argv, "la")) != -1) {
        switch(opt) {
            case 'a':
                flaga = 1;
                break;
            case 'l':
                flagl = 1;
                break;
            case '?':
                // invalid option, error displayed by getopt
                return;
        }
    }
    if(optind == argc) {
        lsDir(".", flagl, flaga);
        return;
    }

    if(optind == argc - 1) {
        char fName[MAX_PATH_LEN];
        if(argv[optind][0] == '~') {
            strcpy(fName, HOME);
            strcat(fName, argv[optind] + 1);
        }
        else
            strcpy(fName, argv[optind]);

        struct stat path_stat;
        stat(fName, &path_stat);
        if(S_ISDIR(path_stat.st_mode))
            lsDir(argv[optind], flagl, flaga);
        else
            lsFile(".", argv[optind], flagl, flaga);
        return;
    }

    vectorString dirNames = init_vectorString();
    int count = 0;
    for(; optind < argc; optind++) {
        char fName[MAX_PATH_LEN];
        if(argv[optind][0] == '~') {
            strcpy(fName, HOME);
            strcat(fName, argv[optind] + 1);
        }
        else
            strcpy(fName, argv[optind]);

        struct stat path_stat;
        stat(fName, &path_stat);
        if(S_ISDIR(path_stat.st_mode))
            pushbackString(&dirNames, argv[optind]);
        else {
            lsFile(".", argv[optind], flagl, 1);
            count++;
        }
    }
    for(int i = 0; i < dirNames.size; i++) {
        if(count > 0) {
            printf("\n");
        }
        printf("%s:\n", dirNames.vector[i]);
        lsDir(dirNames.vector[i], flagl, flaga);
        count++;
    }
    free_vectorString(dirNames);
}
