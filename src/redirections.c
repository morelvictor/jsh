#include "redirections.h"

enum redir {a,b,c,d,e,f,g,h,i};

int redirect(int r, char * path){
	int fd;
	switch(r){
		case a : 
			fd=open(path,O_RDONLY,0600);
			if(fd!=-1) dup2(fd,STDIN_FILENO);
			return fd;
		case b : 
			fd=open(path,O_WRONLY | O_CREAT | O_EXCL,0600);
			if(fd!=-1)dup2(fd,STDOUT_FILENO);
			return(fd);
		case c :
			fd=open(path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
			if(fd!=-1) dup2(fd,STDOUT_FILENO);
			return(fd);
		case d : 
			fd=open(path, O_WRONLY | O_CREAT | O_APPEND, 0600);
			if(fd!=1) dup2(fd,STDOUT_FILENO);
			return fd;
		default : return -1;
			
	}
}

int main(void){
	redirect(b,"fic");
	execlp("ls","ls",NULL);
	/*int fd =open("fic",O_WRONLY | O_CREAT,0600);
	dup2(fd,STDOUT_FILENO);
	write(STDOUT_FILENO, "jana", 4);
	close(fd);
	write(STDOUT_FILENO, "ayadi", 5);*/
	exit(0);
}

