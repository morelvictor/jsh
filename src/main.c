#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <unistd.h>

#include "parser.h"

int main() {

	char *input;
	w_index *index;
	int pid;

	rl_initialize();

	while(1) {
		input = readline("> ");
		index = split_space(input);
		if(index->size == 0) {
			continue;
		} else {
			pid = fork();

			if(pid == 0) {
				execvp(index->words[0], index->words);

				perror("Probleme");

			} else {
				int status;

				waitpid(pid, &status, 0);

				if (WIFEXITED(status)) {
					char *status_str = malloc(4*sizeof(char)); // status <= 256 donc 3 char pour le nombre et 1 pour \0
					sprintf(status_str, "%d", WEXITSTATUS(status));
					setenv("?", status_str, 1);
      		} else {
         		printf("La commande s'est terminée de manière anormale\n");
      		}

			}
		}
	}

	return 0;
}
