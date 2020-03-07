/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roalvare <roalvare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 15:06:04 by mchardin          #+#    #+#             */
/*   Updated: 2020/03/07 17:38:47 by roalvare         ###   ########.fr       */
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

int			run_command(t_shell *shell)
{
	int ret;

	shell->command = get_command(shell->tab[0]);
	if (shell->command == EXEC)
	{
		ret = executable(shell);
		if (ret == 127)
			ft_dprintf(2, "minishell : %s: %s\n", shell->tab[0], ERR_CMD_NF);
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

int			main_loop(t_shell *shell)
{
	shell->pipeline = NULL;
	if (analyse_args(shell))
	{
		if (!(last_arg_env(&shell->env_keys, &shell->env_items, shell->tab)))
			exit_error(shell, shell->tab[0]);
		if (ft_lstsize(shell->pipeline) == 1)
			shell->stop = run_command(shell);
		else if (ft_lstsize(shell->pipeline) > 1)
			loop_pipe(shell);
		if (!shell->stop && ft_lstsize(shell->pipeline) == 1)
			ft_putstr_fd(shell->output, shell->fd_output);
	}
	ft_freez((void **)&shell->output);
	ft_lstclear(&shell->pipeline, free_cmd);
	return (shell->arg.sep == END_LINE ? 0 : 1);
}

// __attribute__((destructor)) void lul(void) // A EFFACER
// {
// 	system("leaks minishell");
// }

int			main(int argc, char **argv, char **envp)
{
	t_shell		shell;
	int			keepreading;
	char		*line;
	struct stat stats;
	
	fstat(0, &stats);
	// signal(SIGQUIT, SIG_IGN);
	ft_mainargs(argc, argv, envp, &shell);
	while (1)
	{
		ft_putstr_fd(PROMPT, shell.fd_line);
		// signal(SIGINT, )
		while ((keepreading = get_next_line(shell.fd_line, &line)) >= 0)
		{
			shell.line[0] = ft_strjoin_gnl(shell.line[0], line);
			ft_freez((void**)&line);
			if (shell.fd_line || S_ISFIFO(stats.st_mode) || S_ISREG(stats.st_mode) || keepreading == 1 || !shell.line[0][0])
				break ;
		}
		if (keepreading < 0)
			exit_error(&shell, 0);
		shell.cursor[0] = shell.line[0];
		while (main_loop(&shell))
			;
		if (!keepreading)
			exit_end(&shell);
		free_line(&shell);
	}
	exit_end(&shell);
}
