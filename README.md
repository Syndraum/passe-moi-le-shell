Cas chelous :

> ./a.out | echo "loulou"
> echo loulou \ //OSEF
> echo loulou" \ //OSEF
> echo loulou \; pwd
> cd "lib //OSEF
> cd /tmp
> pour export si la variable existe deja faut la remplacer donc faut pas juste rajouter a la fin. // OK
> cd ../MINISHELL a bugué une fois (No such file or directory)
> minishell$ cd ../test >> minishell: cd: ../tst : No such file or directory (il a enlevé le e, pourquoi??)
> si on supprime le pwd et qu'on refait pwd, faut afficher comme si il existait toujours. // OK
> si on supprine le pwd et qu'on fait cd ../dir_name faut aller dans .Trash // OK
> echo pwd | bash
> unset OLDPWD ou PWD >> n'est plus jamais recréé si on les redefinie pas a la main
> minishell$ echo 
[1]    37831 segmentation fault  ./minishell
> export sans arg (liste avec declare -x et les def entre guillements, sauf _=...) // OK (sauf keys alone)
> cd sans argument >> $HOME // OK
> executables : la casse doit etre ignoree
> ecrire le pwd si on passe par CDPATH
> minishell$ echo "a
[1]    18675 segmentation fault  ./minishell
> minishell$ cd ~
[1]    18804 segmentation fault  ./minishell //OK
> minishell$ cd ""
[1]    27948 segmentation fault  ./minishell
> peut etre faut gerer les var d'env dans un dictionnaire (exemple : export lala, export >>> declare -x lala apparait) (exemple2 : le OLDPWD qui se met pas a jour si il est unset. La key existe plus. Si on unset PWD, OLDPWD devient une chaine vide, OLDPWD se remet a jour meme si on unset PWD....)
> bash: gkrdgjfd: command not found // minishell : command not found >> rajouter le nom de l'argument
> checker tous les return
> faire la fn de exit error (free)
> fn free hors erreur
> POUR ROMAIN <3 : faut que t'implementes ~ au lieu de $HOME dans les args
Erreurs a pas oublier de gerer :

> redirections vers un directory au lieu d'un file
> cd .. lala : string not in pwd : .. //OSEF
