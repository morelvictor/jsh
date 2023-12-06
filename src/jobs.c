#include "jobs.h"

const char * str_of_state(state st) {
	switch(st){
		case Running: return "Running";
		case Stopped: return "Stopped";
		case Detached: return "Detached";
		case Killed: return "Killed";
		case Done: return "Done";
		default: return "Inconnu";
	}
}

int print_job(job job) {
	printf("[%d] %d\t%s\t%s\n", job.id, job.pgrp, str_of_state(job.st), job.cmd);
	return 0;
}

int print_jobs(job *jobs, int *opt, ...) {
	if(!opt[0]) {
		if(opt[1] == 0){
			for(int i = 0; i < MAX_JOBS; i++) {
				if(jobs[i].st != Done) {
					print_job(jobs[i]);
				}
			}
		} else {
			for(int i = 1; opt[i] != 0; i++) {
				print_job(jobs[i]);
			}
		}
	}

	return 0;
}

// Un job dont tous les processus sont terminés
bool procs_finished(job job) {
	
	return false;
}

// Tous les processus directement lancés par le shell 
// ont correctement terminé(avec exit)
bool shell_procs_exited(job job) {

	return false;
}

// Au moins un processus directement lancé par le shell
// a terminé suite à la réception d'un signal
bool shell_procs_sigend(job job) {

	return false;
}

// Au moins un processus n'a pas terminé <=> non procs_finished

// Tous les processus directement lancés par le shell ont terminé
bool shell_procs_finished(job job) {
	return shell_procs_exited(job) || shell_procs_sigend(job);
}


// Tous les processus non terminés directement lancés par le shell sont suspendus
bool shell_running_procs_suspended(job job) {

	return false;
}


int update_job(job job) {

	return 1;
}

int update(job *jobs) {
	for(int i = 0; i < MAX_JOBS; i++) {
		if(jobs[i].st != Done) {
			update_job(jobs[i]);
		}
	}
	return 0;
}

