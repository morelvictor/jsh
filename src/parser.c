#include "parser.h"
#include "redirections.h"
#include <unistd.h>


void free_index(w_index *pi) {
	for(size_t i = 0; i < pi->size; ++i) {
		free(pi->words[i]);
	}
	free(pi->words);
	free(pi);
}

void print_index(w_index *pi) {
	//printf("nbr: %zu\n", pi->size);
	//printf("words:\n");
	for(size_t i = 0; i < pi->size; ++i) {
		printf("%s\n", pi->words[i]);
	}
}

void print_index_in_line(w_index *pi) {
	//printf("nbr: %zu\n", pi->size);
	//printf("words:\n");
	for(size_t i = 0; i < pi->size; ++i) {
		printf("%s ", pi->words[i]);
	}
	printf("\n");
}

int nbr_words(int (*f)(int), char *s) {
	int i = 0;
	bool pre_s = true;
	
	for(size_t c = 0; s[c] != '\0'; ++c) {
		if(pre_s && !f(s[c]))
			++i;
		pre_s = f(s[c]);
	}
	return i;
}

int word_len(int(*f)(int), const char *w) {
	assert(isalpha(*w) || !f(*w));
	int i = 0;
	
	for(size_t c = 0; (isalpha(w[c]) || !f(w[c])) && w[c] != '\0' ; ++c) {
		++i;
	}
	return i;
}

char *extract_word(int(*f)(int), const char *w, int *pl) {
	assert(isalpha(*w) || !f(*w));
	*pl = word_len(f, w);
	char *c = malloc(*pl * sizeof(char) + *pl);
	c[*pl] = '\0';
	return memmove(c, w, *pl * sizeof(char));
}

char *next_word(int (*f)(int), char *w) {
	char *c = w;
	while(f(*c) || *c == '\0') {
		if(*c == '\0')
			return NULL;
		++c;
	}
	return c;
}

w_index *cons_index(int (*f)(int), char *s) {
	w_index *acc = malloc(sizeof(w_index));
	acc->size = nbr_words(f, s);
	acc->words = malloc((acc->size + 1) * sizeof(char *));
	char *current = next_word(f, s);
	for(int i = 0; current != NULL && i < acc->size; ++i) {
		int pl = -1;
		acc->words[i] = extract_word(f, current, &pl);
		current = next_word(f, current + pl);
	}
	acc->words[acc->size] = NULL;
	return acc;
}

w_index *sub_index(w_index *i, size_t deb, size_t fin) {
  w_index *acc = malloc(sizeof(w_index));
  acc->size = fin-deb;
  acc->words = malloc((acc->size +1) * sizeof(char *));
  for(int x = 0; x < acc->size; ++x) {
    acc->words[x] = malloc((strlen(i->words[x + deb]) + 1) * sizeof(char));
    strcpy(acc->words[x], i->words[x + deb]);
  }
	acc->words[acc->size]= NULL;
  return acc;
}

int is_slash(int c) {
	return c == '/';
}

int is_semicolon(int c) {
	return c == ':';
}

w_index *split_slash(char *s) {
	return cons_index(is_slash, s);
}

w_index *split_space(char *s) {
	return cons_index(isspace, s);
}
w_index *split_semicolon(char *s) {
	return cons_index(is_semicolon , s);
}

redir_index is_redirected(w_index *pi){
	redir_index ri={.redir=-1,.indice=-1};
	for (int i=0; i<pi->size; ++i){
		ri.indice=i;
		if(strcmp(pi->words[i],"<")==0){
			ri.redir=INPUT;
			break;
		}
		else if (strcmp(pi->words[i],">")==0){
			ri.redir=NO_OVERWRITE;
			break;
		}
		else if (strcmp(pi->words[i],">|")==0){
			ri.redir=OVERWRITE;
			break;
		}
		else if (strcmp(pi->words[i],">>")==0){
			ri.redir=CONCAT;
			break;
		}
		else if (strcmp(pi->words[i],"2>")==0){
			ri.redir=ERR_NO_OVERWRITE;
			break;
		}
		else if (strcmp(pi->words[i],"2>|")==0){
			ri.redir=ERR_OVERWRITE;
			break;
		}
		else if (strcmp(pi->words[i],"2>>")==0){
			ri.redir=ERR_CONCAT;
			break;
		}
		
	}
	return ri;
}

int is_redirection_valid(int size, int indice){
	if (size <=2 || indice != size-2) return 0; 
	return 1;
}

int redirect(int redir_type, char * path){
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
}



int is_chevron(char *s){
	if(strcmp(s,"<")==0){
		return INPUT;
	}
	else if (strcmp(s,">")==0){
		return NO_OVERWRITE;
	}
	else if (strcmp(s,">|")==0){
		return OVERWRITE;
	}
	else if (strcmp(s,">>")==0){
		return CONCAT;
	}
	else if (strcmp(s,"2>")==0){
		return ERR_NO_OVERWRITE;
	}
	else if (strcmp(s,"2>|")==0){
		return ERR_OVERWRITE;
	}
	else if (strcmp(s,"2>>")==0){
		return ERR_CONCAT;
	}
	return -1;
	
}

// -3 : NO_REDIRECTION
// -2 : SYNTAX_ERROR
// -1 : FAILED
int check_redirection(w_index *pi){
	int i,j,res;
	
	for(i=0; i<pi->size; ++i){
		res=is_chevron(pi->words[i]);
		if(res!=-1) break;
	}

	if(i==0 || (i==pi->size-1 && res!=-1)) return -2;
	if(i==pi->size && res==-1) return -3;
	int alt=i%2;
	for(j=i; j<pi->size; ++j){
		int res=is_chevron(pi->words[j]);
		if(res==-1){
			if(j%2==alt) return -2;
		}else{
			if(j%2!=alt || j==pi->size-1) return -2;
			int nb=redirect(res,pi->words[j+1]); //ben non imagine y a une erreur sur j+1
			if(nb==-1) return -1;
		}		
	}
	return i;

}

/*int main(){
	//int fd=open("jesuispala",O_RDWR);
	//if(fd==-1) write(1,"message dans stdout\n",20);
	//close(fd);
	w_index *pi=split_space("ls fic1 fic2");
	int nb=check_redirection(pi);
	printf("%d\n",nb);
	//free_index(pi);
	//char *args[]={"ls",NULL};
	//execvp("ls",args);
	//perror("execvp");
	
	exit(0);

}*/




