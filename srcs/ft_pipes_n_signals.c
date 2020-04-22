/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipes_n_signals.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 16:49:23 by mchardin          #+#    #+#             */
/*   Updated: 2020/04/22 21:52:43 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		sig_ignore(int i)
{
	(void)i;
}

void		sig_ctrl_c(int i)
{
	(void)i;
	ft_dprintf(0, "\n%s", PROMPT);
}

void		pipe_child(t_shell *shell, t_pipeline *pipeline)
{
	shell->tab = pipeline->cmd->arg;
	shell->fd_input = pipeline->cmd->fd_input;
	shell->fd_output = pipeline->cmd->fd_output;
	if (shell->fd_input > 2)
		dup2(shell->fd_input, 0);
	else
		dup2(pipeline->save_fd, 0);
	if (shell->fd_output > 2)
		dup2(shell->fd_output, 1);
	else if (pipeline->elmt->next != NULL)
		dup2(pipeline->fd[1], 1);
	close(pipeline->fd[0]);
	shell->stop = run_command(shell);
	ft_putstr_fd(shell->output, 1);
	exit(shell->stop);
}

void		loop_pipe(t_shell *shell)
{
	t_pipeline pipeline;

	pipeline.save_fd = 0;
	pipeline.elmt = shell->pipeline;
	while (pipeline.elmt != NULL)
	{
		pipeline.cmd = (t_cmd*)pipeline.elmt->content;
		pipe(pipeline.fd);
		if ((pipeline.child = fork()) < 0)
			exit(1);
		if (pipeline.child == 0)
			pipe_child(shell, &pipeline);
		else
		{
			waitpid(pipeline.child, &shell->stop, 0);
			shell->stop = WEXITSTATUS(shell->stop);
			close(pipeline.fd[1]);
			pipeline.save_fd = pipeline.fd[0];
			pipeline.elmt = pipeline.elmt->next;
		}
	}
}
