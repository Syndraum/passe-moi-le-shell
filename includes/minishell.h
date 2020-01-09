#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <errno.h>

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

typedef enum    e_argument
{
    TO_FILE = 1, //>
    TO_END, //>>
    FROM_FILE, //<
    SEMICOLON,
    PIPE,
    DOLLAR,
    ECHO_N,
    ARG
}               t_argument;

#endif