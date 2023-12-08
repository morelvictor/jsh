#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "parser.h"
#include "internes.h"
#include "prompt.h"
#include "redirections.h"

int main() {
	int in=dup(0);
	int out=dup(1);
	int err_out=dup(2);
	char *input;
	redir_index ri;
	w_index *maguelonne;
	w_index *index;
	w_index *sub;
	int pid;
	extern char* prompt;
	prompt = malloc(256);
	int res=update_prompt(0);
	rl_initialize();
	rl_outstream=stderr;

	while((input = readline(prompt)) != NULL) {
		if(res) {
			free(input);
			free(prompt);
			exit(10);
		}
		add_history(input);
		index = split_space(input);
		ri = is_redirected(index);
		if (ri.redir!=-1){
			if(is_redirection_valid(index->size,ri.indice)){
				sub=sub_index(index,0,ri.indice);
				maguelonne=sub;
				redirect(ri.redir,index->words[index->size-1]);
				if(maguelonne->size != 0) {
					int ret_code;
					if(strcmp(maguelonne->words[0],"cd")==0){
						if(maguelonne->size==1){
							ret_code = cd(NULL);
						}else{
							ret_code = cd(maguelonne->words[1]);
						}
					} else if(strcmp(maguelonne->words[0],"pwd")==0){
						ret_code = pwd();
					} else if(strcmp(maguelonne->words[0], "?") == 0) {
						ret_code = return_code();
					} else if(strcmp(maguelonne->words[0], "exit") == 0) {
						int exit_code = -1;
						char *last = getenv("?");
						exit_code = (maguelonne->size == 2) ? atoi(maguelonne->words[1]) : last == NULL ? 20 : atoi(last);
						free(input);
						free_index(index);
						free(sub);
						exit(exit_code);
					} else {

						pid = fork();

						if(pid == 0) {
							execvp(maguelonne->words[0], maguelonne->words);

							perror("Probleme");
							exit(1);

						} else {
							int status;

							waitpid(pid, &status, 0);

							if (WIFEXITED(status)) {
								ret_code = WEXITSTATUS(status);
							} else {
								printf("La commande s'est terminée de manière anormale\n");
							}

						}
					}
					char buff[8];
					sprintf(buff, "%d", ret_code);
					setenv("?", buff, 1);
				}


				
			}
			
		}
		else{
		if(index->size != 0) {
			int ret_code;
			if(strcmp(index->words[0],"cd")==0){
				if(index->size==1){
					ret_code = cd(NULL);
				}else{
					ret_code = cd(index->words[1]);
				}
			} else if(strcmp(index->words[0],"pwd")==0){
				ret_code = pwd();
			} else if(strcmp(index->words[0], "?") == 0) {
				ret_code = return_code();
			} else if(strcmp(index->words[0], "exit") == 0) {
				int exit_code = -1;
				char *last = getenv("?");
				exit_code = (index->size == 2) ? atoi(index->words[1]) : last == NULL ? 20 : atoi(last);
				free(input);
				free_index(index);
				exit(exit_code);
			} else {

				pid = fork();

				if(pid == 0) {
					execvp(index->words[0], index->words);

					perror("Probleme");
					exit(1);

				} else {
					int status;

					waitpid(pid, &status, 0);

					if (WIFEXITED(status)) {
						ret_code = WEXITSTATUS(status);
					} else {
						printf("La commande s'est terminée de manière anormale\n");
					}

				}
			}
			char buff[8];
			sprintf(buff, "%d", ret_code);
			setenv("?", buff, 1);
		}
		}
		dup2(in,0);
		dup2(out,1);
		dup2(err_out,2);
		update_prompt(0);
		free(input);
		free_index(index);
	}
	free(prompt);
	char *last = getenv("?");
	return last == NULL ? 0 : atoi(last);
}



