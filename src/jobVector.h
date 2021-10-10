#ifndef C_SHELL_JOBVECTOR_H
#define C_SHELL_JOBVECTOR_H

struct job {
    int pid;
    int jobNum;
    char *pname;
};

typedef struct job_vector {
    struct job *vector;
    int size;
    int capacity;
} vectorJob;

vectorJob init_vectorJob();

void pushbackJob(vectorJob *, int, char *);

char *jobName(vectorJob, int);

void removeJob(vectorJob *, int);

void free_vectorJob(vectorJob);

#endif
