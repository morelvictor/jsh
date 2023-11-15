#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

char *prompt(int jobs, char *pwd) {
	char * prompt=malloc(100);
	
	int diff=0;
	int len_jobs=floor(log10((double)jobs))+1;
	int count=len_jobs+strlen(pwd)+4; /*2 crochets, 1 dollar, 1 espace*/

	if(count>30) {
		diff=count-30+3;
		if(strlen(pwd)>diff) sprintf(prompt,"\001\033[33m\002[%d]\001\033[36m\002...%s\001\033[00m\002$ ",jobs,pwd+diff);
		else printf("oups petit souçi ici...,prompt pas affichable");
		/*meme si on tronque le chemin c est trop long (la faute au nb de jobs) -> peu probable*/
		
	}else{
		sprintf(prompt,"\001\033[33m\002[%d]\001\033[36m\002%s\001\033[00m\002$ ",jobs,pwd);
	}
	
	/*int len_visible=len_jobs+strlen(pwd+diff)+4;
	if(diff>0) len_visible+=3;
	printf("taille visible du prompt (<=30 !!) : %d\n",len_visible);
	printf("taille totale du prompt (avec bascules, taille visible +21) : %ld\n",strlen(prompt));*/
	
	return prompt;
}

int main(){
	char *p=prompt(3,"/home/titi/pas/trop/long/mais/la/ca/depasse");
	printf("%s\n",p);
	char *p2=prompt(17,"/home/titi");
	printf("%s\n",p2);
	free(p);
	free(p2);
	exit(0);
}
