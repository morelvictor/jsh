#include "internes.h"

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

int return_code() {
	char *ret;
	ret = getenv("?");
	if(ret == NULL)
		return 1;
	printf("%s\n", ret);
	return 0;
}


//c est la fonction pour kill
void send_signal(job** jobs,int signal, int target, int job_or_not){
	if (job_or_not==1) {//envoie du signal au job de num target
		if(jobs[target-1]!=NULL){
        		if (kill(-jobs[target-1]->pgid, signal) == -1) {
            			perror("kill");
            			exit(EXIT_FAILURE);
        		}
		}
	} else if (job_or_not==0){
		if(kill(target, signal) == -1) {
			perror("kill");
			exit(EXIT_FAILURE);
        	}
	} else {
        	fprintf(stderr, "La cible est invalide : %d\n", target);
        	exit(EXIT_FAILURE);
    }
}

