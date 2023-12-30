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
int is_substituted(w_index *pi){
	int n=count_substitutions(pi);
	if(n==-1) return -2;
	if(n==0) return 0;
	return n;
}
w_index *substitute(w_index *pi){
	const int n=is_substituted(pi);
	if(n==-1 || n==-2) return NULL;
	if(n==0) return pi;
	int open[n];
	int close[n];
	int *fds=malloc(n*sizeof(int));
	pos_open_sub(pi,open);
	pos_close_sub(pi,close);
	w_index *first=sub_index(pi,0,open[0]);
	w_index **cmds=malloc(n*sizeof(w_index *));
	get_cmds_sub(cmds,pi,open,close,n);
	printf("1\n");
 	for(int i=0; i<n; ++i){
		//printf("i = %d\n",i);
		print_index(cmds[i]);
		fds[i]=launch_cmd(cmds[i]);
	}
	int acc=0;
	int i=open[0];
	while(i<pi->size){
		if(strcmp("<(",pi->words[i])==0){
			char buff[50];
			sprintf(buff, "cat /dev/fd/%d", fds[acc]);
			add_word(first,buff);
			++acc;
			i=close[i]+1;
		}else{
			add_word(first,pi->words[i]);
			++i;
		}
	}
	return first;
}



/*int main(){
	w_index *pi=split_space("cat <( ls . )");
	w_index *nouv=substitute(pi);
	print_index(nouv);
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
