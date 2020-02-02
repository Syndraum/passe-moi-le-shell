#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>

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
	char		*pwd;
	int			stop;
}				t_shell;
// FT_BUILTINS
int			command_pwd(t_shell *shell);
int			command_cd(t_shell *shell);
int			command_export(t_shell *shell);
int			command_unset(t_shell *shell);
int			command_env(t_shell *shell);
// FT_BUILTINS_UTILS
int			replace_var(char **environ, char *var);
void		unset_var(char **environ, char *var);
int			replace_or_add(char ***environ, char *var);
int			pwd_env(t_shell *shell);
int			last_arg_env(char ***environ, char **tab);
// FT_ECHO
char		*skip_if(char *str, int (*f)(char));
int			command_echo(t_shell *shell);
// FT_IS_VAR
int			is_var_ret_idx(char *var);
int			is_same_var(char *var, char *vardef);
int			is_var_to_unset(char *var, char *vardef);
int			is_var_def(char *var);
// FT_ARG_INTERPRETATION
int			is_end_of_command(t_separator sep);
int			is_redirection(t_separator sep);
int			init_struct(t_shell *shell);
int			ft_redirection(t_shell *shell, t_separator prev);
int			analyse_args(t_shell *shell);
// FT_ARG_TRANSLATION
void		*set_arg(t_shell *shell);
// FT_ARG_TRANSLATION_UTILS
int			is_stoparg(char c);
int			is_stop_noquote(char c);
int			is_stop_dquote(char c);
int			strlen_to(char *str, char stop);
int			strlen_if(char *str, int (*f)(char));
// FT_CURSOR_UTILS
void   		skip_char(char **cursor, char c);
int			ft_cmp(char *str, char *cursor);
int			cmp_skip(char **cursor, char *str);
int			get_arg(char **cursor);

#endif