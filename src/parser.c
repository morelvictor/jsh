#include "parser.h"
#include "redirections.h"


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
/*int is_chevron(char *s){
	if(strcmp(pi->words[i],"<")==0){
		return INPUT;
	}
	else if (strcmp(pi->words[i],">")==0){
		return NO_OVERWRITE;
	}
	else if (strcmp(pi->words[i],">|")==0){
		return OVERWRITE;
	}
	else if (strcmp(pi->words[i],">>")==0){
		return CONCAT;
	}
	else if (strcmp(pi->words[i],"2>")==0){
		return ERR_NO_OVERWRITE;
	}
	else if (strcmp(pi->words[i],"2>|")==0){
		return ERR_OVERWRITE;
	}
	else if (strcmp(pi->words[i],"2>>")==0){
		return ERR_CONCAT
	}
	return -1;
	
}*/
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
/*redir_index is_redirected(w_index *pi){
	redir_index ri={.redir=-1,.indice=-1};
	for(int i=0; i<pi->size; ++i){
		int res=is_chevron(pi->words[i]);
		if(res!=-1){
			ri.redir=res;
			ri.indice=i;
			return ri;
		}
	}
	return ri;

}*/
/*enum {DONE,SYNTAX_ERROR,FAILED};
int is_redirection_valid(w_index *pi){
	redir_index ri=is_redirected(pi);
	int alt=ri.indice%2;
	for(int i=ri.indice; i<pi->size; ++i){
		int res=is_chevron(pi->words[i]);
		if(res!=-1){
			if(i%2!=alt || i=pi->size-1) return SYNTAX_ERROR;
			
	}

}*/

int is_redirection_valid(int size, int indice){
	if (size <=2 /*minimum 3 composantes*/ || indice != size-2 /*+ d'1 mot apres la redir*/) return 0; 
	/*char *fic_name = pi->word[pi->size-1];
	for(int i=0; i<strlen(fic_name); ++i){
	}*/
	return 1;
}
/*int is_redirection_valid(int size, int indice){
si l'indice du symbole de redirection est en position 0 : syntaxe invalide/too few arguments (dans le projet)
sinon, s'il y a plus d'1 mot apres le symbole de redirection : too many arguments
sinon : LA SYNTAXE est correcte (ce qui n'exclu pas que exec echoue  par la suite)
}*/

/*int main(){

	w_index *pi=split_space("ls lala lala lalal > fic");
	redir_index ri=is_redirected(pi);
	printf("redirection de type %d en position %d\n",ri.redir,ri.indice);
	printf("%d\n", is_redirection_valid(pi->size, ri.indice));
	free_index(pi);
	exit(0);

}*/

