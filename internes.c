#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


void cd(char *path) {
    if (chdir(path) != 0) { /*chdir utilisée pour changer le 
    répertoire de travail actuel du processus.La fonction chdir renvoie 0
     en cas de succès et -1 en cas d'échec.*/
        perror("cd");
    }
}


void pwd() {
    char poss[2028];
    if (getcwd(poss, sizeof(poss)) != NULL) {
        printf("%s\n", poss);
    } else {
        perror("getcwd");
    }
}

