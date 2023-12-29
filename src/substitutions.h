#ifndef SUBSTITUTIONS_H
#define SUBSTITUTIONS_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "parser.h"

extern int count_substitutions(w_index *);
extern void pos_open_sub(w_index *,int *);
extern void pos_close_sub(w_index *,int *);
extern void print_tab(int *,int);
extern void get_cmds_sub(w_index **, w_index *, int *, int *, int );

#endif
