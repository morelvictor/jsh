#ifndef JOBS_H
#define JOBS_H

#include <stdio.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <stdbool.h>

#define MAX_JOBS 500

enum State {
		Running,
		Stopped,
		Detached,
		Killed,
		Done
	};

typedef enum State state;

struct Process {
	struct Process *next;	/* Next process in the pipeline.  A circular chain. */
	pid_t pid;					/* Process ID. */
	int running;				/* Non-zero if this process is running. */
	char *command;				/* The particular program that is running. */
};

typedef struct Process process;

struct Job {
		int id;
		int pgrp;
//		process *pipe;			/* En prévision des pipes */
		state st;
		char cmd[260];
	};

typedef struct Job job;

int update(job *);
int print_jobs(job *, int *, ...);

#endif
