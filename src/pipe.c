#include "pipe.h"

int count_pipe(w_index *pi){
	int acc=0;
	for(int i=0; i<pi->size; ++i){
		if(strcmp("|",pi->words[i])==0){
			++acc;
		}
	}
	return acc;
}

void pos_pipe(w_index *pi, int *t){
	int acc=0;
	for(int i=0; i<pi->size; ++i){
		if(strcmp("|",pi->words[i])==0){
			t[acc]=i;
			++acc;
		}
	}
}

void get_cmds_pipe(w_index *pi,w_index **cmds, int n_pipes){
	//const int n=count_pipe(pi);
	int n = n_pipes; //TODO
	int pos[n];
	pos_pipe(pi,pos);
	
	int deb=0;
	int fin=0;
	// attention : n symboles pipe = n+1 commandes
	for(int i=0; i<n+1; ++i){
		if(i==n) fin=pi->size;
		else fin=pos[i];
		cmds[i]=sub_index(pi,deb,fin);
		deb=pos[i]+1;
	}
	
}

