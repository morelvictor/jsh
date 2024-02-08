# JSH - Un Shell avec Job Control

## Introduction

Bienvenue dans JSH, un shell en C axé sur la simplicité et l'efficacité pour les développeurs qui cherchent une solution légère avec un contrôle basique des tâches.

## Fonctionnalités Principales

### Contrôle Basique des Tâches

JSH propose des fonctionnalités de contrôle des tâches simples pour faciliter la gestion des processus en cours d'exécution.

```bash
$ jsh
$ gcc mon_programme.c -o mon_programme
$ ./mon_programme &
$ jobs          # Affiche la liste des tâches en cours d'exécution
$ bg %1         # Relance une tâche en arrière-plan
$ fg %1         # Reprend l'exécution d'une tâche en premier plan
$ stop %1       # Met en pause une tâche
$ kill %1       # Arrête une tâche
```

### Compilation avec Make

Vous pouvez compiler `jsh` simplement en faisant la commande:

```bash
$ make          # Compile le projet sans complications
```

## Installation

1. Clonez le dépôt : `git clone https://github.com/votre-utilisateur/jsh.git`
2. Accédez au répertoire : `cd jsh`
3. Compilez avec `make`

## Utilisation

1. Exécutez le shell : `./jsh`
2. Profitez des fonctionnalités de base de contrôle des tâches et de la syntaxe simplifiée.

## Contribution

Les contributions sont les bienvenues. JSH est un projet en évolution constante, et toute amélioration est appréciée.

## Avertissement

JSH est un shell en développement continu. Veuillez noter que des fonctionnalités expérimentales peuvent être présentes. Utilisez-le en conséquence.
