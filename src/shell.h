#ifndef SHELL_H
#define SHELL_H

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern int shell_pgid;
extern int shell_terminal;

extern void init_shell();

#endif
