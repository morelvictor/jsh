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

int print_job(job *job) {
	printf("[%d] %d\t%s\t%s\n", job->id, job->pgid, str_of_state(job->state), job->cmd);
	return 0;
}

int print_jobs(job **jobs, int size) {
	for(int i = 0; i < size; ++i) {
		print_job(jobs[i]);
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

	if((st & 0b11000) == 0b11000) {
		job->state = DONE;
		return 0;
	}
	if((st & 0b10100) == 0b10100) {
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

int update_jobs(job **jobs, int size) {
	for(int i = 0; i < MAX_JOBS; i++) {
		if(jobs[i] != NULL) {
			if(jobs[i]->state != DONE && jobs[i] ->state != KILLED && jobs[i]->state != DETACHED) {
				update_job(jobs[i]);
			}
		}
	}
	return 0;
}

int launch_job() {
	return -1;
}
