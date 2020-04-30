/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipes_n_signals.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 16:49:23 by mchardin          #+#    #+#             */
/*   Updated: 2020/04/30 15:22:17 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		sig_ignore(int i)
{
	(void)i;
}

void		sig_ctrl_s(int i)
{
	(void)i;
	STOP = 131;
}

void		sig_ctrl_c(int i)
{
	(void)i;
	ft_dprintf(0, "\n%s", PROMPT);
	STOP = 130;
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
	STOP = run_command(shell);
	ft_putstr_fd(shell->output, 1);
	exit(STOP);
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
			waitpid(pipeline.child, &STOP, 0);
			STOP = WEXITSTATUS(STOP);
			close(pipeline.fd[1]);
			pipeline.save_fd = pipeline.fd[0];
			pipeline.elmt = pipeline.elmt->next;
		}
	}
}
