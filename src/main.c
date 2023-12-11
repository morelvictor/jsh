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
	w_index *current;
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
		//printf("0\n");
		index = split_space(input);
		//printf("1\n");
		redir_index ri = is_redirected(index);
		//printf("%d\n",ri.redir);
		//printf("ici\n");
		if (ri.redir!=-1){
			//printf("là\n");
			if(is_redirection_valid(index->size,ri.indice)){
				sub=sub_index(index,0,ri.indice);
				current=sub;
				redirect(ri.redir,index->words[index->size-1]);
			}
			
		}else{
			//printf("2\n");
			current=index;
		}
		//printf("3\n");

					

		if(current->size != 0) {
			int ret_code;
			if(strcmp(current->words[0],"cd")==0){
				if(current->size==1){
					ret_code = cd(NULL);
				}else{
					ret_code = cd(current->words[1]);
				}
			} else if(strcmp(current->words[0],"pwd")==0){
				ret_code = pwd();
			} else if(strcmp(current->words[0], "?") == 0) {
				ret_code = return_code();
			} else if(strcmp(current->words[0], "exit") == 0) {
				int exit_code = -1;
				char *last = getenv("?");
				exit_code = (current->size == 2) ? atoi(current->words[1]) : last == NULL ? 20 : atoi(last);
				free(input);
				free_index(current);
				exit(exit_code);
			} else {

				pid = fork();

				if(pid == 0) {
					execvp(current->words[0], current->words);

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
		dup2(in,0);
		dup2(out,1);
		dup2(err_out,2);
		update_prompt(0);
		free(input);
		free_index(current);
	}
	free(prompt);
	char *last = getenv("?");
	return last == NULL ? 0 : atoi(last);
}



