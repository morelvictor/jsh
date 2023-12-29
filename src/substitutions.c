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
	/*int fd[2];
	if(pipe(fd)==-1){
		perror("pipe failed");
		exit(1);
	}
	close(fd[0]);*/
	int fd=open("yeah", O_WRONLY | O_CREAT | O_EXCL, 0600);
	switch(fork()){
		case -1 : 
			perror("fork failed");
			exit(1);
		case 0 : 
			dup2(fd,1);
			execvp(pi->words[0],pi->words);
			perror("execvp");
			exit(1);
		default : 
			wait(NULL);
			
	}
	return fd;
}
int launch_cmd2(w_index *pi){
	switch(fork()){
		case -1 : 
			perror("fork failed");
			exit(1);
		case 0 :
			execvp(pi->words[0],pi->words);
			perror("execvp");
			exit(1);
		default : 
			wait(NULL);
			
	}
	return 0;
}

/*int main(){
	w_index *pi=split_space("ls .");
	int fd=launch_cmd(pi);
	printf("descripteur : %d\n", fd);
	char buffer[50];
	sprintf(buffer,"cat /dev/fd/%d",fd);
	printf("%s\n",buffer);
	w_index *ti=split_space(buffer);
	launch_cmd2(ti);

	w_index *p=split_space("cmd1 <( cmd2 ) <( cmd3 | cmd3.1 )");
	w_index *pi=sub_index(p,0,4);
	pi->words=realloc(pi->words,(pi->size+1)*sizeof(char*));
	char *nouv=malloc(5*sizeof(char));
	nouv[0]='j';
	nouv[1]='a';
	nouv[2]='n';
	nouv[3]='a';
	nouv[4]='\0';
	pi->words[pi->size]=malloc((strlen(nouv)+1)*sizeof(char));
	strcpy(pi->words[pi->size],nouv);
	pi->size=pi->size+1;
	free(nouv);
	free(p);
	print_index(pi);
	free_index(pi);

	
	const int n=count_substitutions(pi);
	if(n==0) printf("pas de substitution\n");
	if (n==-1) printf("syntaxe invalide\n");
	if(n>0){
		int t[n];
		int p[n];
		pos_open_sub(pi,t);
		pos_close_sub(pi,p);
		//print_tab(t,n);
		//print_tab(p,n);
		w_index **cmds=malloc(n*(sizeof(w_index *)));
		get_cmds_sub(cmds,pi,t,p,n);
		for(int i=0; i<n; ++i){
			print_index(cmds[i]);
		}
		
		char *s=malloc(5*sizeof(char));
		s[0]='j';
		s[1]='a';
		s[2]='n';
		s[3]='a';
		char *cp=malloc(5*sizeof(char));
		strcpy(cp,s);
		s[0]='n';
		free(s);
		printf("%s\n",cp);	

	}
	exit(0);
}*/
