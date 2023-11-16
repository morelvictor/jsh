#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int cd(char *path) {
    char *nouv;

    if (path == NULL) {
        nouv = getenv("HOME"); //getenv recup val associee a une var d'env donnee
        if (nouv == NULL) {
            fprintf(stderr, "Erreur : $HOME n'est pas défini.\n");
            return 1;
        }
    } else if (strcmp(path, "-") == 0) {
        nouv = getenv("OLDPWD");
        if (nouv == NULL) {
            fprintf(stderr, "Erreur : le répertoire précédent n'est pas défini.\n");
            return 1;
        }
    } else {
        nouv = path;
    }

    if (chdir(nouv) == 0) {
        setenv("OLDPWD", getenv("PWD"), 1);
        setenv("PWD", nouv, 1);
        return 0;
    } else {
        perror("chdir");
        return 1;  
    }
}



int pwd() {
    char poss[2028];
    if (getcwd(poss, sizeof(poss)) != NULL) {
        printf("%s\n", poss);
	return 0;
    } else {
        perror("getcwd");
	return 1;
    }
}

