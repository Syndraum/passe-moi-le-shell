#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"

typedef enum    e_command
{
    EXEC = 1,
    ECHO,
    CD,
    PWD,
    EXPORT,
    UNSET,
    ENV,
    EXIT
}               t_command;


#endif