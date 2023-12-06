#ifndef REDIRECTIONS_H
#define REDIRECTIONS_H
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

enum redir {
	INPUT,
	NO_OVERWRITE,
	OVERWRITE,
	CONCAT,
	ERR_NO_OVERWRITE,
	ERR_OVERWRITE,
	ERR_CONCAT,
	PIPE,
	SUBSTITUTION
};
	

extern int redirect(int,char *);

#endif
