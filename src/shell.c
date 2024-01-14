#include "shell.h"

job *jobs[MAX_JOBS];
int ret_code=0;
w_index *current_index = NULL;
int fds[1024];

