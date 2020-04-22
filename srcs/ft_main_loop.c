/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_main_loop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/20 19:54:20 by mchardin          #+#    #+#             */
/*   Updated: 2020/04/22 21:52:08 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		cmd_loop(t_shell *shell)
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

void	line_utils(t_shell *shell)
{
	error_strings(shell);
	shell->cursor[0] = shell->line[0];
	shell->cursor2[0] = shell->line[0];
	if (check_arg(shell))
		while (cmd_loop(shell))
			;
}

int		line_loop(t_shell *shell, struct stat stats)
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
			if (shell->fd_line || S_ISFIFO(stats.st_mode) ||
				S_ISREG(stats.st_mode))
				shell->line_nb++;
			break ;
		}
	}
	if (keepreading < 0)
		exit_error(shell, 0);
	line_utils(shell);
	if (!keepreading)
		exit_end(shell);
	free_line(shell);
	return (1);
}
