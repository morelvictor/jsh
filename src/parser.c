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
  acc->words = malloc(acc->size * sizeof(char *));
  for(int x = 0; x < acc->size; ++x) {
    acc->words[x] = malloc((strlen(i->words[x + deb]) + 1) * sizeof(char));
    strcpy(acc->words[x], i->words[x + deb]);
  }
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
int is_redirected(w_index *pi){
	for (int i=0; i<pi->size; ++i){
		if(strcmp(pi->words[i],"<")==0) return INPUT;
		else if (strcmp(pi->words[i],">")==0) return NO_OVERWRITE;
		else if (strcmp(pi->words[i],">|")==0) return OVERWRITE;
		else if (strcmp(pi->words[i],">>")==0) return CONCAT;
		else if (strcmp(pi->words[i],"2>")==0) return ERR_NO_OVERWRITE;
		else if (strcmp(pi->words[i],"2>|")==0) return ERR_OVERWRITE;
		else if (strcmp(pi->words[i],"2>>")==0) return ERR_CONCAT;
		
	}
	return -1;

}
/*int main(){

	w_index *pi=split_space("ls src > fic");
	w_index *pi_sub=sub_index(pi,0,1);
	print_index(pi);
	puts("");
	print_index(pi_sub);
	free_index(pi);
	free_index(pi_sub);
	exit(0);

}*/


