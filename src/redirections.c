#include "redirections.h"

/*int redirect(int redir_type, char * path){
	int fd;
	switch(redir_type){
		case INPUT : 
			fd=open(path,O_RDONLY,0600);
			if(fd!=-1) dup2(fd,STDIN_FILENO);
			return fd;
		case NO_OVERWRITE : 
			fd=open(path,O_WRONLY | O_CREAT | O_EXCL,0600);
			if(fd!=-1)dup2(fd,STDOUT_FILENO);
			return(fd);
		case OVERWRITE :
			fd=open(path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
			if(fd!=-1) dup2(fd,STDOUT_FILENO);
			return(fd);
		case CONCAT : 
			fd=open(path, O_WRONLY | O_CREAT | O_APPEND, 0600);
			if(fd!=1) dup2(fd,STDOUT_FILENO);
			return fd;
		case ERR_NO_OVERWRITE : 
			fd=open(path,O_WRONLY | O_CREAT | O_EXCL, 0600);
			if(fd!=-1) dup2(fd,STDERR_FILENO);
			return fd;
		case ERR_OVERWRITE :
			fd=open(path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
			if(fd!=-1) dup2(fd,STDERR_FILENO);
			return fd;
		case ERR_CONCAT : 
			fd=open(path,O_WRONLY | O_CREAT | O_APPEND, 0600);
			if(fd!=-1) dup2(fd, STDERR_FILENO);
			return fd;
		default : return -1;
			
	}
}*/

/*int main(void){
	int fd=redirect(NO_OVERWRITE,1,"fic1");
	int fd2=redirect(NO_OVERWRITE,fd,"fic2");
	execlp("ls","ls",NULL);
	int fd =open("fic",O_WRONLY | O_CREAT,0600);
	dup2(fd,STDOUT_FILENO);
	write(STDOUT_FILENO, "jana", 4);
	close(fd);
	write(STDOUT_FILENO, "ayadi", 5);
	exit(0);
}*/

