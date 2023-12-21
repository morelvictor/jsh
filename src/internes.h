#ifndef INTERNES_H
#define INTERNES_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

extern int cd(char *path);
extern int pwd();
extern int return_code();
extern void send_signal(int signal, int target, int job_or_not);

#endif
