#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <prompt.h>

char *prompt(int jobs) {
	char *prompt=malloc(256);
	char *path=malloc(256);
	if(getcwd(path,256)==NULL) {
		perror("bla");
		exit(1);
	}	
	
	int diff=0;
	int len_jobs=1; // minimum 0 jobs -> 0 est le longueur 1
	if(jobs>0) len_jobs=floor(log10((double)jobs))+1;
	int count=len_jobs+strlen(path)+4; //2 crochets, 1 dollar, 1 espace

	if(count>30) {
		diff=count-30+3;
		if(strlen(path)>diff) sprintf(prompt,"\001\033[33m\002[%d]\001\033[36m\002...%s\001\033[00m\002$ ",jobs,path+diff);
		
	}else{
		sprintf(prompt,"\001\033[33m\002[%d]\001\033[36m\002%s\001\033[00m\002$ ",jobs,path);
	}
	
	
	return prompt;
}

int main(){
	printf("[0]------------------------>$ \n");
	char *p=prompt(3);
	printf("%s\n",p);
	free(p);
	exit(0);
}
