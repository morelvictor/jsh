#ifndef SUBSTITUTIONS_H
#define SUBSTITUTIONS_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "parser.h"

extern int count_substitutions(w_index *);
extern void pos_open_sub(w_index *,int *);
extern void pos_close_sub(w_index *,int *);
extern void print_tab(int *,int);
extern void get_cmds_sub(w_index **, w_index *, int *, int *, int );
extern int launch_cmd(w_index *);
extern int launch_test(w_index *);
extern int is_substituted(w_index *);
extern w_index * substitute(w_index *);

#endif
