# Cas chelous

- [x] ~~./a.out | echo "loulou"~~
- [x] ~~echo loulou \ //OSEF~~
- [x] ~~echo loulou" \ //OSEF~~
- [x] ~~echo loulou \; pwd~~
- [x] ~~cd "lib //OSEF~~
- [ ] cd /tmp
- [x] ~~pour export si la variable existe deja faut la remplacer donc faut pas juste rajouter a la fin. // OK~~
- [ ] cd ../MINISHELL a bugué une fois (No such file or directory)
- [ ] minishell$ cd ../test >> minishell: cd: ../tst : No such file or directory (il a enlevé le e, pourquoi??)
- [x] ~~si on supprime le pwd et qu'on refait pwd, faut afficher comme si il existait toujours. // OK~~
- [x] ~~si on supprine le pwd et qu'on fait cd ../dir_name faut aller dans .Trash // OK~~
- [x] ~~echo pwd | bash~~
- [ ] unset OLDPWD ou PWD >> n'est plus jamais recréé si on les redefinie pas a la main
- [x] ~~minishell$ echo 
[1]    37831 segmentation fault  ./minishell~~
- [x] ~~export sans arg (liste avec declare -x et les def entre guillements, sauf _=...) // OK (sauf keys alone)~~
- [x] ~~cd sans argument >> $HOME // OK~~
- [ ] executables : la casse doit etre ignoree
- [ ] ecrire le pwd si on passe par CDPATH
- [x] ~~minishell$ echo "a
[1]    18675 segmentation fault  ./minishell~~
- [x] ~~minishell$ cd ~
[1]    18804 segmentation fault  ./minishell //OK~~
- [ ] minishell$ cd ""
[1]    27948 segmentation fault  ./minishell
- [ ] peut etre faut gerer les var d'env dans un dictionnaire (exemple : export lala, export >>> declare -x lala apparait) (exemple2 : le OLDPWD qui se met pas a jour si il est unset. La key existe plus. Si on unset PWD, OLDPWD devient une chaine vide, OLDPWD se remet a jour meme si on unset PWD....)
- [ ] bash: gkrdgjfd: command not found // minishell : command not found >> rajouter le nom de l'argument
- [ ] checker tous les return
- [ ] faire la fn de exit error (free)
- [ ] fn free hors erreur
- [x] POUR ROMAIN <3 | MERCI BEAUCOUP <3 (non.): faut que t'implementes ~ au lieu de $HOME dans les args
Erreurs a pas oublier de gerer :

- [ ] redirections vers un directory au lieu d'un file
- [x] ~~cd .. lala : string not in pwd : .. //OSEF~~

# Cher journal

**JOUR 1**
```
Aujourd'hui j'ai mis en place le pipeline sous forme de liste chainé !
Tant que mon séparateur est un pipe j'initialise le tableau d'argument et je l'ajoute à ma liste
Du coup tab dans la structure shell correspond au dernier tableau d'argument rencontré dans le pipeline
Ex: echo test;
Le pipeline ne contient qu'une commande, tab contient la dernière et seule commande
Ex: echo test | echo lol;
seul echo lol sera exécuté pour l'instant :'(
```

**JOUR 2**
```
Il c'est passé beaucoup de chose !
Déjà... Les pipes fonctionnent !
J'utilise la longueur de la liste chainé pour savoir si j'exécute le pipe ou non !
Puis je lance ma boucle de pipe ! un pipe entre chaque processus
Avant d'executer une commande, je change le tableau d'arg !
C'est moche mais sinon j'aurais du changer chaque build-in ><'
Autre truc que j'ai géré, le retour des processus !
Le numéro retourné lors de l'exécution d'un pipe et d'un exécutable est bon
Et dernier truc la redirection sortante lors d'un pipe !
Bon c'est pas ouf ouf comment on fait mais on verra ça après :)
```