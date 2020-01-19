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
> si on supprime le pwd et qu'on refait pwd, faut afficher comme si il existait toujours. 
> si on supprine le pwd et qu'on fait cd ../dir_name faut aller dans .Trash

Erreurs a pas oublier de gerer :

> redirections vers un directory au lieu d'un file
> cd .. lala : string not in pwd : .. //OSEF