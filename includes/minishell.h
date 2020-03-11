#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>

# define FILE_RIGHTS S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
# define PROMPT "\033[0;32mminishell$ \033[0m"
# define ERR_HOME "HOME not set"
# define ERR_CMD_NF "command not found"
# define ERR_ID "not a valid identifer"
# define ERR_TOKEN "syntax error near unexpected token"
# define ENV_COMMAND "_=env\n"
typedef enum	e_command
{
	EXEC = 1,
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT
}				t_command;

typedef enum	e_separator
{
	END_LINE,
	TO_FILE, //>
	TO_END, //>>
	FROM_FILE, //<
	SEMICOLON,
	PIPE,
	ARG
}				t_separator;

typedef enum	e_signal
{
	NONE,
	CTRL_C,
	CTRL_D,
	CTRL_K
}				t_signal;
typedef struct	s_cmd
{
	char		**arg;
	int			fd_input;
	int			fd_output;
}				t_cmd;


typedef struct s_arg
{
	char		*str;
	t_separator	sep;
}				t_arg;

typedef	struct	s_shell
{
	char		**tab;
	char		*output;
	char		*cursor[1];
	char		*cursor2[1];
	char		*line[1];
	int			fd_line; // fd de lecture des commandes
	int			fd_input;
	int			fd_output;
	t_command	command;
	t_arg		arg;
	char		**env_keys;
	char		**env_items;
	char		*pwd;
	char		*oldpwd;
	char		*lastarg;
	int			stop;
	t_list		*pipeline;
	t_signal	sig;			
}				t_shell;
// FT_MINISHELL //
int			get_command(char *command);
int			run_command(t_shell *shell);
int			cmd_loop(t_shell *shell);
// FT_INIT //
char		*ft_shlvl(t_shell *shell);
void		ft_open_line(char *file, t_shell *shell);
void		ft_first_init_struct(t_shell *shell);
void		ft_mainargs(int argc, char **argv, char **envp, t_shell *shell);
// FT_INIT_ENV //
int			ft_env_lib(t_shell *shell, char **env);
int			replace_item(char **keys, char **items, char *key, char *item);
int			init_oldpwd(char ***keys, char ***items);
// FT_ARG_INTERPRETATION //
int			ft_redirection(t_shell *shell, t_separator prev);
int			arg_loop(t_shell *shell);
int			analyse_args(t_shell *shell);
// FT_ARG_INTERPRETATION_UTILS //
int			is_end_of_command(t_separator sep);
int			is_redirection(t_separator sep);
void		init_struct(t_shell *shell);
int			init_tab(t_shell *shell);
// FT_ARG_TRANSLATION //
char		*get_dollar(char **cursor, t_shell *shell, char stop_char);
char		*get_quote(char **cursor);
void		*get_tilde(char **cursor, t_shell *shell);
void		*get_argument(char **cursor, t_shell *shell);
char		*set_arg(t_shell *shell);
// FT_ARG_TRANSLATION_UTILS //
int			is_stoparg(char c);
int			is_stop_unquote(char c);
int			is_stop_dquote(char c);
int			strlen_to(char *str, char stop);
int			strlen_if(char *str, int (*f)(char));
// FT_ARG_DQUOTE //
char		*strncmp_esc_dquote(char *dest, char *src, int len);
char		*get_tmp_dquote(char **cursor, char *dquote, int len, int i);
char		*get_dollar_dquote(char **cursor, t_shell *shell, char *dquote);
char		*init_dquote(char **cursor, int *len, int *i);
char		*get_dquote(char **cursor, t_shell *shell);
// FT_ARG_UNQUOTE //
char		*strncmp_esc(char *dest, char *src, int len);
char		*get_dollar_unquote(char **cursor, t_shell *shell, char *arg);
char		*get_unquote(char **cursor, t_shell *shell);
// FT_CURSOR_UTILS //
void   		skip_char(char **cursor, char c);
int			ft_cmp(char *str, char *cursor);
int			cmp_skip(char **cursor, char *str);
int			get_sep(char **cursor);
// FT_BUILTINS //
int			command_pwd(t_shell *shell);
int			command_cd(t_shell *shell);
int			command_export(t_shell *shell);
int			command_unset(t_shell *shell);
int			command_env(t_shell *shell);
// FT_BUILTINS_UTILS //
char		*home_path(char **keys, char **items);
int			cd_path(t_shell *shell, char *dir);
char		*print_export(char **keys, char **items);
// FT_ENV_UTILS //
int			check_split_var(char *var, char **key, char **item);
int			unset_var(char **keys, char **items, char *key);
int			replace_or_add(char ***keys, char ***items, char *key, char *item);
void		pwd_env(t_shell *shell);
char		*last_arg_env(t_shell *shell, char **tab);
// FT_ECHO //
char		*skip_if(char *str, int (*f)(char));
int			command_echo(t_shell *shell);
// FT_EXECUTABLE //
int			count_malloc_env(char **keys, char **items);
char		**convert_env(t_shell *shell, char *path);
char		*ft_lowercase(char *str);
int			ft_strncmp_case(char *s1, char *s2, size_t n);
char		*try_path(char *filename, char *path_dir);
char		*getpath(t_shell *shell);
int			change_exec(t_shell *shell, char *path);
int			launch_exec(t_shell *shell, char *path);
int			executable(t_shell *shell);
// FT_IS_VAR //
int			is_var_ret_idx(char *var);
int			is_key_var(char *key, char *var);
int			is_var_to_unset(char *var, char *vardef);
int			is_var_def(char *var);
// FT_PIPES_N_SIGNALS //
void		sig_ignore(int i);
void		sig_ctrl_c(int i);
void		loop_pipe(t_shell *shell);
// FT_FREE_CLOSE //
void		free_line(t_shell *shell);
void		free_cmd(void *tab);
void		free_all(t_shell *shell);
void		close_all(t_shell *shell);
// FT_EXIT //
void		exit_error(t_shell *shell, char *fonction);
void		exit_end(t_shell *shell);
// FT_FIND_ENV //
int			get_tabidx(char *key, char **tab);
char		*get_item(char *key, char **keys, char **items);
#endif
