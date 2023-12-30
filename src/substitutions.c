#include "substitutions.h"
#include "parser.h"



int count_substitutions(w_index* pi){
	int n=0;
	int m=0;
	for(int i=0; i<pi->size; ++i){
		if(strcmp("<(",pi->words[i])==0) ++n;
		if(strcmp(")",pi->words[i])==0) ++m;
	}
	if(n!=m) return -1;
	else return n;
}

void pos_open_sub(w_index *pi, int *t){
	int acc_t=0;	
	for(int i=0; i<pi->size; ++i){
		if(strcmp("<(",pi->words[i])==0){
			t[acc_t]=i;
			++acc_t;
		}
	}
}
void pos_close_sub(w_index *pi,int *t){
	int acc_t=0;	
	for(int i=0; i<pi->size; ++i){
		if(strcmp(")",pi->words[i])==0){
			t[acc_t]=i;
			++acc_t;
		}
	}
}
void print_tab(int *t,int len){
	for(int i=0; i<len; ++i){
		printf("%d  ",*(t+i));
	}
	puts("");
}
void get_cmds_sub(w_index **cmds, w_index *pi, int *t, int *p, int n){
	for(int i=0; i<n; ++i){
		cmds[i]=sub_index(pi,t[i]+1,p[i]);
	}
	
}
int launch_cmd(w_index *pi){
	int out=dup(1);
	int fd[2];
	if(pipe(fd)==-1){
		perror("pipe failed");
		exit(1);
	}
	switch(fork()){
		case -1 : 
			perror("fork failed");
			exit(1);
		case 0 : 
			dup2(fd[1],1);
			execvp(pi->words[0],pi->words);
			perror("execvp");
			exit(1);
		default : 
			read(fd[0],NULL,0);
			close(fd[0]);
			dup2(out,1);
			close(out);
			
			
	}
	return fd[1];
}
int launch_test(w_index *pi){
	switch(fork()){
		case -1 : 
			perror("fork");
			exit(1);
		case 0 : 
			execvp(pi->words[0],pi->words);
			perror("execvp");
			exit(1);
		default : 
			wait(NULL);
			break;
	}
	return 0;
}

//-1 : FAILED
//-2 : SYNTAX ERROR
// 0 : NO SUBSTITUTION
int is_subsituted(w_index *pi){
	int n=count_substitutions(pi);
	if(n==-1) return -2;
	if(n==0) return 0;
	return n;
}



/*int main(){
	w_index *pi=split_space("ls .");
	int fd=launch_cmd(pi);
	printf("descripteur : %d\n", fd);
	char buff[50];
	sprintf(buff, "cat /dev/fd/%d", fd);
	w_index *ti=split_space(buff);
	launch_test(ti);
	close(fd);
	exit(0);
}*/
