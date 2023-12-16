#ifndef JOBS_H
#define JOBS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <stdbool.h>

#define MAX_JOBS 500

typedef enum state {
	DONE,
	KILLED,
	DETACHED,
	STOPPED,
	RUNNING
} state;

typedef struct process {
	struct process *next;
	pid_t pid;
	int status; //-1 tant que il a pas été initialisé
	char *command;
} process;


typedef struct job {
	int id;
	int pgid;
	process *pipeline;
	state state;
	char *cmd;
} job;

int update_jobs(job **, int);
int print_jobs(job **, int);
int launch_job();

#endif
