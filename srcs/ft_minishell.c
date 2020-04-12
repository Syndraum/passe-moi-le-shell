/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roalvare <roalvare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 15:06:04 by mchardin          #+#    #+#             */
/*   Updated: 2020/03/11 19:50:41 by roalvare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			get_command(char *command)
{
	if (ft_cmp(command, "echo"))
		return (ECHO);
	else if (ft_cmp(command, "cd"))
		return (CD);
	else if (ft_cmp(command, "pwd"))
		return (PWD);
	else if (ft_cmp(command, "export"))
		return (EXPORT);
	else if (ft_cmp(command, "unset"))
		return (UNSET);
	else if (ft_cmp(command, "env"))
		return (ENV);
	else if (ft_cmp(command, "exit"))
		return (EXIT);
	else
		return (EXEC);
}

void		print_error(t_shell *shell, char *s2, char *s1)
{
	ft_dprintf(2, "%s%s: %s%s", shell->error_beg, s2, s1, shell->error_line);
}

int			run_command(t_shell *shell)
{
	int ret;

	if (shell->fd_input == -1 || shell->fd_output == -1)
		return (1);
	shell->command = get_command(shell->tab[0]);
	if (shell->command == EXEC)
	{
		ret = executable(shell);
		if (ret == 127)
			print_error(shell, shell->tab[0], ERR_CMD_NF);
		else if (ret == 126)
			print_error(shell, shell->tab[0], ERR_FILE_DIR);
		return (ret);
	}
	else if (shell->command == ECHO)
		return (command_echo(shell));
	else if (shell->command == CD)
		return (command_cd(shell));
	else if (shell->command == PWD)
		return (command_pwd(shell));
	else if (shell->command == EXPORT)
		return (command_export(shell));
	else if (shell->command == UNSET)
		return (command_unset(shell));
	else if (shell->command == ENV)
		return (command_env(shell));
	else if (shell->command == EXIT)
		exit_end(shell);
	return (1);
}

char		*get_separator(int sep)
{
	if (sep == TO_FILE)
		return (">");
	else if (sep == TO_END)
		return (">>");
	else if (sep == FROM_FILE)
		return ("<");
	else if (sep == SEMICOLON)
		return (";");
	else if (sep == PIPE)
		return ("|");
	return (NULL);
}

int			check_arg(t_shell *shell)
{
	char	**cursor;
	char	*sep;

	cursor = shell->cursor2;
	shell->arg.sep = 0;
	while (**cursor)
	{
		*cursor = skip_if(*cursor, ft_iswhitespace);
		shell->arg.str = get_argument(cursor, shell);
		shell->arg.sep = get_sep(cursor);
		if (!shell->arg.str && shell->arg.sep)
		{
			free(shell->arg.str);
			sep = get_separator(shell->arg.sep);
			print_error(shell, ERR_TOKEN, sep);
			return (0);
		}
		ft_freez((void**)&shell->arg.str);
		*cursor = skip_if(*cursor, ft_iswhitespace);
	}
	if (!shell->arg.str && is_redirection(shell->arg.sep))
	{
		print_error(shell, ERR_TOKEN, "'mewline'");
		return (0);
	}
	return (1);
}

int			cmd_loop(t_shell *shell)
{
	shell->pipeline = NULL;
	if (analyse_args(shell))
	{
		if (ft_lstsize(shell->pipeline) == 1)
			shell->stop = run_command(shell);
		else if (ft_lstsize(shell->pipeline) > 1)
			loop_pipe(shell);
		if (!shell->stop && ft_lstsize(shell->pipeline) == 1)
			ft_putstr_fd(shell->output, shell->fd_output);
	}
	shell->lastarg = last_arg_env(shell, shell->tab);
	unset_var(shell->env_keys, shell->env_items, "_");
	ft_freez((void **)&shell->output);
	ft_lstclear(&shell->pipeline, free_cmd);
	return (shell->arg.sep == END_LINE ? 0 : 1);
}

int			line_loop(t_shell *shell, struct stat stats)
{
	char		*line;
	int			keepreading;

	ft_putstr_fd(PROMPT, shell->fd_line);
	while ((keepreading = get_next_line(shell->fd_line, &line)) >= 0)
	{
		shell->line[0] = ft_strjoin_gnl(shell->line[0], line);
		ft_freez((void**)&line);
		if (shell->fd_line || S_ISFIFO(stats.st_mode) || S_ISREG(stats.st_mode)
			|| keepreading == 1 || !shell->line[0][0])
		{
			if (shell->fd_line || S_ISFIFO(stats.st_mode) || S_ISREG(stats.st_mode))
				shell->line_nb++;
		}
		break ;
	}
	if (keepreading < 0)
		exit_error(shell, 0);
	error_strings(shell);
	shell->cursor[0] = shell->line[0];
	shell->cursor2[0] = shell->line[0];
	if (check_arg(shell))
		while (cmd_loop(shell))
			;
	if (!keepreading)
		exit_end(shell);
	free_line(shell);
	return (1);
}

int			main(int argc, char **argv, char **envp)
{
	t_shell		shell;
	struct stat stats;

	signal(SIGINT, sig_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
	fstat(0, &stats);
	ft_mainargs(argc, argv, envp, &shell);
	while (line_loop(&shell, stats))
		;
	exit_end(&shell);
}
