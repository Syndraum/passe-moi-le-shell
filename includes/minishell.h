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

typedef enum    e_separator
{
	END_LINE,
    TO_FILE, //>
    TO_END, //>>
    FROM_FILE, //<
    SEMICOLON,
    PIPE,
    ARG
}               t_separator;

typedef struct s_arg
{
	char		*str;
	t_separator	sep;
}				t_arg;

typedef	struct	s_shell
{
	char		**tab;
	char		**output;
	char		**cursor;
	int			fd;
	t_arg		arg;
}				t_shell;

int     cmp_skip(char **cursor, char *str);

char	*skip_if(char *str, int (*f)(char));
char	*command_echo(char **cursor);

int		is_whitespace(char c);
char	*get_argument(char **str);

#endif