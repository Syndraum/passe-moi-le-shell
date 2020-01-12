#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <errno.h>
# include <fcntl.h>

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
	char		*output;
	char		**cursor;
	int			fd;
    t_command   command;
	t_arg		arg;
	char		**environ;
}				t_shell;

void		command_pwd(t_shell *shell);
int			command_cd(t_shell *shell);
int			command_export(t_shell *shell);
int			command_unset(t_shell *shell);
void		command_env(t_shell *shell);

int			is_same_var(char *var, char *vardef);
int			is_var_to_unset(char *var, char *vardef);
int			is_var_def(char *var);

int			cmp_skip(char **cursor, char *str);
int			get_arg(char **cursor);

char		*skip_if(char *str, int (*f)(char));
char		*command_echo(t_shell *shell);

int			is_whitespace(char c);
void		*set_arg(t_shell *shell);

int			init_struct(t_shell *shell);
int			analyse_args(t_shell *shell);

#endif