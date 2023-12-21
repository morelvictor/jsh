#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "parser.h"
#include "internes.h"
#include "prompt.h"
#include "jobs.h"
#include "shell.h"
#include "redirections.h"

int main() {

	int in=dup(0);
	int out=dup(1);
	int err_out=dup(2);
	char *input;
	w_index *current;
	w_index *index;
	w_index *sub;
	int nb;
	extern char* prompt;
	prompt = malloc(256);
	int res=update_prompt(0);
	rl_initialize();
	rl_outstream=stderr;
	init_shell();
	jobs = calloc(MAX_JOBS, sizeof(job *));
	int fg = 1;
	while((input = readline(prompt)) != NULL) {
		int ret_code=0;
		if(res) {
			free(input);
			free(prompt);
			exit(10);
		}
		add_history(input);
		index = split_space(input);
		if(index->size != 0) {
			if(!strcmp(index->words[index->size - 1], "&")) {
				fg = 0;
				w_index *new_i = sub_index(index, 0, index->size - 1);
				//free_index(index);
				index = new_i;
				//print_index(index);
			}
		}
		current=index;
		nb=check_redirection(index);
		if(nb==-2 || nb== -1) {
			ret_code=1;
			goto end_loop;
		}
		if(nb>=0){
			sub=sub_index(index,0,nb);
			current=sub;
		}
		
		if(current->size != 0) {
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
			} else if(strcmp(index->words[0], "exit") == 0){

				// Vérifier si des jobs sont en cours d'exécution ou suspendus
   		 		if (are_jobs_running(jobs)) {
        				printf("Des jobs sont en cours d'exécution. Impossible de exit.\n");
        				free_index(index);
        				return 1;
    				}

    				if (current->size == 1) {
        			// Pas d'argument pour exit, terminer le shell
        				char *last = getenv("?");
        				int value = atoi(last);
        				free_index(index);
        				exit(value);
    				} else if (index->size == 2) {
        			// terminer le shell avec la valeur de retour spécifiée
        				int value = atoi(current->words[1]);
        				free_index(index);
        				exit(value);
    				} else {
        			//erreur
        				goto end_loop;
    				}




			} else if(strcmp(index->words[0], "kill")==0) {
				int signal;
				int target;
				int job_or_not=1;

				if (index->size<2 || index->size>3){
					goto end_loop;
				}
				if(index->size==2){
					signal=SIGTERM;
					if(index->words[1][0]!='%'){
						target=atoi(index->words[1]);
						job_or_not=0;
					} else {
						//memmove(index->words[1], index->words[1] + 1, strlen(index->words[1]));
						//target = atoi(index->words[1]);
						target=atoi(index->words[1]+1);
					}
				} else {
				//	signal=(index->words[1][1]=='\0')? (index->words[1][1]- '0') : ((index->words[1][1] - '0') * 10 + (index->words[1][2] - '0'));
					signal= atoi(index->words[1]+1);
					if(index->words[2][0]!='%'){
                                                target=atoi(index->words[2]);
						job_or_not=0;
                                        } else {
                                               // memmove(index->words[2], index->words[2]+ 1, strlen(index->words[2]));
                                               // target = atoi(index->words[2]);
						target=atoi(index->words[2]+1);
                               		}
				}
				send_signal(jobs, signal, target, job_or_not);



			} else if(strcmp(index->words[0], "jobs") == 0) {
				ret_code = print_jobs(jobs);
			} else {
				int status;
				int pid = exec_command(input, current, fg, jobs);
				if(fg) {
					waitpid(pid, &status, 0);
					if (WIFEXITED(status)) {
						ret_code = WEXITSTATUS(status);
					} else {
						printf("La commande s'est terminée de manière anormale\n");
					}
					//tcsetpgrp(STDIN_FILENO, getpid());
				}
			}
end_loop:
			char buff[8];
			sprintf(buff, "%d", ret_code);
			setenv("?", buff, 1);
		}
		dup2(in,0);
		dup2(out,1);
		dup2(err_out,2);
		//peut mieux faire ici, mieux que remettre les 3 (2 sont inutiles)
		update_jobs(jobs);
		update_prompt(count_jobs(jobs));
		free(input);
		free_index(current);
		if(nb>0) free_index(index);
		fg = 1;
	}
	free(prompt);
	free_jobs(jobs);
	//free(jobs);
	char *last = getenv("?");
	return last == NULL ? 0 : atoi(last);
}



