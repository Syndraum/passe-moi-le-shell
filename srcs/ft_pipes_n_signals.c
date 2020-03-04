/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipes_n_signals.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 16:49:23 by mchardin          #+#    #+#             */
/*   Updated: 2020/03/04 19:33:51 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void		sig_ctrl_c(t_shell *shell)
// {
// 	shell->sig = CTRL_C;
// }

void		loop_pipe(t_shell *shell)
{
	int		fd[2];
	int		fd_input[2];
	pid_t	child;
	int		save_fd;
	t_list	*elmt;
	t_cmd 	*cmd;

	save_fd = 0;
	elmt = shell->pipeline;
	while (elmt != NULL)
	{
		cmd = (t_cmd*)elmt->content;
		pipe(fd);
		if ((child = fork()) < 0)
			exit(1);
		if (child == 0)
		{
			shell->tab = cmd->arg;
			shell->fd_input = cmd->fd_input;
			shell->fd_output = cmd->fd_output;
			if (shell->fd_input != 0)
			{
				pipe(fd_input);
				dup2(shell->fd_input, 0);
				close(fd_input[0]);
			}
			dup2(save_fd, 0);
			if (shell->fd_output != 1)
				dup2(shell->fd_output, 1);
			else if (elmt->next != NULL)
				dup2(fd[1], 1);
			close(fd[0]);
			shell->stop = run_command(shell);
			ft_putstr_fd(shell->output, 1);
			exit(shell->stop);
		}
		else
		{
			waitpid(child, &shell->stop, 0);
			shell->stop = WEXITSTATUS(shell->stop);
			close(fd[1]);
			save_fd = fd[0];
			elmt = elmt->next;
		}
	}
}
