#include "jobs.h"
#include "redirections.h"
#include "parser.h"
#include "pipe.h"

const char * str_of_state(state st) {
	switch(st){
		case RUNNING: return "Running";
		case STOPPED: return "Stopped";
		case DETACHED: return "Detached";
		case KILLED: return "Killed";
		case DONE: return "Done";
		default: return "Unknown";
	}
}

int print_job(FILE *out, job *job, int id) {
	fprintf(out, "[%d]\t%d\t%s\t%s\n", id, job->pgid, str_of_state(job->state), job->cmd);
	return 0;
}

int print_jobs(job **jobs) {
	update_jobs(stdout, jobs);
	int vide = 1;
	for(int i = 0; i < MAX_JOBS; ++i) {
		if(jobs[i] != NULL) {
			print_job(stdout, jobs[i], i);
			vide &= 0;
		}
	}
	/*if(vide) {
		int fd = open("/dev/pts/283", O_WRONLY);
		write(fd, "Vide\n", 5);
	}*/
	return 0;
}


int count_jobs(job **jobs) {
	int acc = 0;
	for(int i = 0; i < MAX_JOBS; i++) {
		if(jobs[i] != NULL)
			++ acc;
	}
	return acc;
}

int update_pipeline(process *pipeline) {
	for(process *curr = pipeline; curr != NULL; curr = curr->next){
		//if(WIFEXITED(curr->status) || WIFSIGNALED(curr->status)) {
		int status;
		switch(waitpid(curr->pid, &status, WNOHANG | WUNTRACED | WCONTINUED)){
			case -1: /*exit(100);*/ break;
			case 0: break;
			default:
				curr->status = status;
		}
		//}
	}
	return 0;
}

void remove_job(job **jobs, job *j) {
	for(int i = 0; i < MAX_JOBS; ++i) {
		if(jobs[i] == j) {
			free_job(j);
			jobs[i] = NULL;
		}
	}
}

/*

   1) Tous les processus sont terminés
   2) Tous les processus directement lancés par le shell ont exit
   3) Au moins un processus directement lancé par le shell a terminé par signal
   4) Au moins un processus n'a pas terminé
   5) Tous les processus directement lancés par le shell ont terminés
   6) Tous les processus non terminés directement lancés par le shell sont suspendus

 */

int update_job(FILE *out, job **jobs, job *job, int id) {
	state old_state = job->state;
	update_pipeline(job->pipeline);
	// 123456
	int st = 0b110011;
	if(kill(- job->pgid, 0) != -1){
		st &= 0b01111; // 1
		//printf("st1: %b\n", st);
	}
	else {
		st |= 0b000100; // 4
		//printf("st4: %b\n", st);
	}
	for(process *curr = job->pipeline; curr != NULL; curr = curr->next){
		if(WIFSIGNALED(curr->status)) {
			st |= 0b001000; // 3
		//printf("st3: %b\n", st);
		}
		if(!WIFEXITED(curr->status)) {
			st &= 0b101111; // 2
		//printf("st2: %b\n", st);
		}
		if(!WIFEXITED(curr->status) && !WIFSIGNALED(curr->status)) {
			st &= 0b111101; // 5
		//printf("st5: %b\n", st);
		}
		if(!WIFEXITED(curr->status) && !WIFSIGNALED(curr->status) && !WIFSTOPPED(curr->status)) {
			st &= 0b111110; //6
		//printf("st6: %b\n", st);
		}
	}

	if((st & 0b110000) == 0b110000) {
		job->state = DONE;
		if(!job->fg) {
			print_job(out, job, id);
			remove_job(jobs, job);
		} else {
			//if(tcsetpgrp(STDIN_FILENO, getpgid(getpid())) == -1) exit(250);
		}
		return 0;
	}
	if(((st & 0b101000) == 0b101000)) {
		job->state = KILLED;
		if(!job->fg)
			print_job(out, job, id);
		remove_job(jobs, job);
		return 0;
	}

		
	if(((st & 0b000001) == 0b000001)) {
		job->state = STOPPED;
		job->fg = 0;
		if(job->state != old_state)
			print_job(out, job, id);
		return 0;
	}
	
	if((st & 0b000110) == 0b000110) {
		job->state = DETACHED;
		if(job->state != old_state)
			print_job(out, job, id);

		return 0;
	}

	job->state = RUNNING;
	if(job->state != old_state)
		print_job(out, job, id);

	return 0;
}

int update_jobs(FILE *out , job **jobs) {
	for(int i = 0; i < MAX_JOBS; i++) {
		if(jobs[i] != NULL) {
			if(jobs[i]->state != DONE && jobs[i]->state != KILLED && jobs[i]->state != DETACHED) {
				update_job(out, jobs, jobs[i], i);
			}
		}
	}
	return 0;
}

void job_fg(job *j) {
	tcsetpgrp(STDIN_FILENO, j->pgid);
}

void launch_process(process *p, int pgid, int fg, int shell_pgid, w_index *index) {
	int pid = getpid();
	if(pgid == 0) pgid = pid;
	if(!fg) setpgid(pid, pgid);
	else setpgid(pid, shell_pgid);
	/*
	   signal (SIGINT, SIG_DFL);
	   signal (SIGQUIT, SIG_DFL);
	   signal (SIGTSTP, SIG_DFL);
	   signal (SIGTTIN, SIG_DFL);
	   signal (SIGTTOU, SIG_DFL);
	   signal (SIGCHLD, SIG_DFL);
	 */
	//check_redirection(index);
	//if(tcgetpgrp(STDIN_FILENO) == pgid) fprintf(stderr, "Oui\n");
	execvp(index->words[0], index->words);
	perror("execvp");
	exit(234);
}

int launch_job(job *j, int fg, w_index *index, int id, int n_pipes) {
	process *p;
	int pid;
	int shell_pgid = getpgid(getpid());
	int pipes[n_pipes][2];
	for(int i = 0; i < n_pipes; i++) {
		pipe(pipes[i]);
	}
	
	int i = 0;
	for(p = j->pipeline; p; p = p->next) {
		if((pid = fork()) != 0) {
			
			p->pid = pid;
			if(!j->pgid) {
				j->pgid = pid;
			}
			setpgid(pid, j->pgid);
			if(!fg) {
				print_job(stderr, j, id);
			}

		} else {
			if(i > 0) {
				dup2(pipes[i-1][0], STDIN_FILENO);
			}
			if(i <= n_pipes-1) {
				dup2(pipes[i][1],STDOUT_FILENO);
			}
			for(int j = 0; j < n_pipes; ++j) {
				close(pipes[j][0]);
				close(pipes[j][1]);
			}
			int nb=check_redirection(p->cmd_index);
			if(nb==-2 || nb== -1) {
				perror("redirections");
				exit(1);
			}
			if(nb>=0){
				p->cmd_index = sub_index(p->cmd_index,0,nb);
			}

			launch_process(p, j->pgid, fg, shell_pgid, p->cmd_index);
		}
		++i;
	}
	if(pid) {
		for(i = 0; i < n_pipes; i++) {
			close(pipes[i][0]);
			close(pipes[i][1]);
		}
	}
	return pid;
}

job *exec_command(char *cmd, w_index *index, int fg, job **jobs) {
	job *new_job = malloc(sizeof(job));
	new_job->cmd = concat(index);
	new_job->state = RUNNING;
	new_job->fg = fg;
//	process *first_process = malloc(sizeof(process));
//	first_process->cmd = concat(index);

	// On génère le tableau des pipes
	int n_pipes = count_pipe(index);
	w_index **cmds = malloc(sizeof(w_index *) * (n_pipes + 1));
	get_cmds_pipe(index, cmds, n_pipes);

	// Construction de la pipeline
	process *current = NULL;
	for(int i = 0; i <= n_pipes; ++i) {
		if(current != NULL) {
			current->next = malloc(sizeof(process));
			current = current->next;
		} else {
			current = malloc(sizeof(process));
			new_job->pipeline = current;
		}
		current->cmd = concat(cmds[i]);
		current->status = -1;
		current->cmd_index = cmds[i];
		current->pid = 0;
	}
	current->next = NULL;
	free(cmds);

	//first_process->next = NULL;
	//first_process->status = -1;
	//first_process->cmd_index = index;
	new_job->pgid = 0;
	int id = -1;
		for(int i = 0; i < MAX_JOBS; ++i) {
			if(jobs[i] == NULL) {
				jobs[i] = new_job;
				id = i;
				break;
			}
		}
	new_job->id = id;
	launch_job(new_job, fg, index, id, n_pipes);
	return new_job;
}

void free_process(process *p){
	if(p->next!=NULL) free_process(p->next);
	free(p->cmd);
	free_index(p->cmd_index);
	free(p);
}

void free_job(job *j){
	free_process(j->pipeline);
	free(j->cmd);
	free(j);
}

void free_jobs(job **jobs){
	for(int i=0; i<MAX_JOBS; ++i){
		if(jobs[i]!=NULL) free_job(jobs[i]);
	}
	free(jobs);
}


int are_jobs_running(job **jobs) {
    if(count_jobs(jobs)>0)return 1;
    return 0;  // Aucun job en cours d'exécution ou suspendu
}
