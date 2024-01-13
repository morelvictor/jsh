#ifndef SHELL_H
#define SHELL_H

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "jobs.h"

extern job *jobs[MAX_JOBS];
extern int ret_code;

#endif
