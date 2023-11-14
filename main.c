#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "internes.h"

int main() {
    char aRegarder[100];

    while (1) {
        printf("Entrez la commande cd avec votre chemin choisi ou pwd: ");

        fgets(aRegarder, sizeof(aRegarder), stdin);

        // Supprimer le saut de ligne à la fin de la commande
        size_t size = strlen(aRegarder);
        if (size > 0 && aRegarder[size - 1] == '\n') {
            aRegarder[size - 1] = '\0';
        }

        // Analysons la commande ...
        char *morceau = strtok(aRegarder, " ");
        if (morceau != NULL) {
            if (strcmp(morceau, "cd") == 0) {
                // Si la commande est "cd", appeler la fonction de changement de répertoire
                morceau = strtok(NULL, " ");
                if (morceau != NULL) {
                    cd(morceau);
                } else {
                    fprintf(stderr, "cd: argument manquant\n");
                }
            } else if (strcmp(morceau, "pwd") == 0) {
                pwd();
            }
        }
    }

    return 0;
}



