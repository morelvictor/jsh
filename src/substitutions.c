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

/*int main(){
	w_index *pi=split_space("cmd1 <( cmd2 ) <( cmd3 )");
	const int n=count_substitutions(pi);
	if(n==0) printf("pas de substitution\n");
	if (n==-1) printf("syntaxe invalide\n");
	if(n>0){
		int t[n];
		int p[n];
		pos_open_sub(pi,t);
		pos_close_sub(pi,p);
		print_tab(t,n);
		print_tab(p,n);

	}
	exit(0);
}*/
