#include "jobs.h"

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

int print_job(job *job, int id) {
	printf("[%d] %d\t%s\t%s\n", id, job->pgid, str_of_state(job->state), job->cmd);
	return 0;
}

int print_jobs(job **jobs) {
	update_jobs(jobs);
	for(int i = 0; i < MAX_JOBS; ++i) {
		if(jobs[i] != NULL)
			print_job(jobs[i], i);
	}
	return 0;
}

int update_pipeline(process *pipeline) {
	for(process *curr = pipeline; curr != NULL; curr = curr->next){
		if(WIFEXITED(curr->status) || WIFSIGNALED(curr->status)) {
			int status;
			switch(waitpid(curr->pid, &status,WNOHANG)){
				case -1: exit(100); break;
				case 0: break;
				default:
					curr->status = status;
			}
		}
	}
	return 0;
}

/*

1) Tous les processus sont terminés
2) Tous les processus directement lancés par le shell ont exit
3) Au moins un processus directement lancé par le shell a terminé par signal
4) Au moins un processus n'a pas terminé
5) Tous les processus directement lancés par le shell ont terminés
6) Tous les processus non terminés directement lancés par le shell sont suspendus

*/

int update_job(job *job) {
	update_pipeline(job->pipeline);
		// 123456
	int st = 0b110011;
	if(kill(job->pgid, 0) != -1) st &= 0b01111; // 1
	else st |= 0b000100; // 4
	for(process *curr = job->pipeline; curr != NULL; curr = curr->next){
		if(WIFSIGNALED(curr->status))
			st |= 0b001000; // 3
		if(!WIFEXITED(curr->status))
			st &= 0b101111; // 2
		if(!WIFEXITED(curr->status) && !WIFSIGNALED(curr->status))
			st &= 0b111101; // 5
		if(!WIFEXITED(curr->status) && !WIFSIGNALED(curr->status) && !WIFSTOPPED(curr->status))
			st &= 0b111110; // 6
	}

	if((st & 0b110000) == 0b110000) {
		job->state = DONE;
		return 0;
	}
	if((st & 0b101000) == 0b101000) {
		job->state = KILLED;
		return 0;
	}
	if((st & 0b000110) == 0b000110) {
		job->state = DETACHED;
		return 0;
	}
	if((st & 0b000001) == 0b000001) {
		job->state = STOPPED;
		return 0;
	}
	job->state = RUNNING;
	return 0;
}

int update_jobs(job **jobs) {
	for(int i = 0; i < MAX_JOBS; i++) {
		if(jobs[i] != NULL) {
			if(jobs[i]->state != DONE && jobs[i]->state != KILLED && jobs[i]->state != DETACHED) {
				update_job(jobs[i]);
			}
		}
	}
	return 0;
}

void job_fg(job *j) {
	tcsetpgrp(STDIN_FILENO, j->pgid);
}

void launch_process(process *p, int pgid, int fg, w_index *index) {
	int pid = getpid();
	if(pgid == 0) pgid = pid;
	setpgid(pid, pgid);
	if(fg) {
//		tcsetpgrp(STDIN_FILENO, pgid);
	}
	/*
	signal (SIGINT, SIG_DFL);
	signal (SIGQUIT, SIG_DFL);
	signal (SIGTSTP, SIG_DFL);
	signal (SIGTTIN, SIG_DFL);
	signal (SIGTTOU, SIG_DFL);
	signal (SIGCHLD, SIG_DFL);
	*/
	execvp(index->words[0], index->words);
	exit(234);
}

int launch_job(job *j, int fg, w_index *index) {
	process *p;
	int pid;
	
	for(p = j->pipeline; p; p = p->next) {
		if((pid = fork()) != 0) {
			p->pid = pid;
			if(!j->pgid) {
				j->pgid = pid;
			}
			setpgid(pid, j->pgid);
		} else {
			launch_process(p, j->pgid, fg, index);
		}
	}
	if(fg) {
//		tcsetpgrp(STDIN_FILENO, j->pgid);
	}
	return pid;
}

int exec_command(char *cmd, w_index *index, int fg, job **jobs) {
	job *new_job = malloc(sizeof(job));
	new_job->cmd = cmd;
	new_job->state = unknown;
	process *first_process = malloc(sizeof(process));
	first_process->cmd = malloc(strlen(cmd) + 1);
	strcpy(first_process->cmd, cmd);
	first_process->next = NULL;
	first_process->status = -1;
	first_process->cmd_index = index;
	new_job->pipeline=first_process;
	new_job->pgid = 0;
	int pid = launch_job(new_job, fg, index);
	for(int i = 0; i < MAX_JOBS; ++i) {
		if(jobs[i] == NULL) {
			jobs[i] = new_job;
			return pid;
		}
	}
	exit(252);
}
