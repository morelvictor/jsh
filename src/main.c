#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "parser.h"
#include "internes.h"
#include "prompt.h"

int main() {
	
	char *input;
	w_index *index;
	int pid;
	extern char* prompt;
	prompt = malloc(256);
	int res=update_prompt(0);
	rl_initialize();
	rl_outstream=stderr;

	while((input = readline(prompt)) != NULL) {
		add_history(input);
		index = split_space(input);
		if(index->size != 0) {
			if(strcmp(index->words[0],"cd")==0){
				if(index->size==1){
					cd(NULL);
				}else{
					cd(index->words[1]);
				}
			} else if(strcmp(index->words[0],"pwd")==0){
				pwd();
			} else if(strcmp(index->words[0], "?") == 0) {
				return_code();
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
						free(status_str);
					} else {
						printf("La commande s'est terminée de manière anormale\n");
					}

				}
			}
		}

		update_prompt(0);
		free(input);
		free_index(index);
	}
	free(prompt);
	clear_history();
	return 0;
}



