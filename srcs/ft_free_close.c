/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_close.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 17:00:30 by mchardin          #+#    #+#             */
/*   Updated: 2020/04/22 21:53:22 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		error_strings(t_shell *shell)
{
	if (shell->line_nb == 0)
	{
		shell->error_beg = ft_strdup("minishell : ");
		shell->error_line = ft_strdup("\n");
	}
	else
	{
		shell->error_beg = ft_sprintf("minishell : line %d: ", shell->line_nb);
		shell->error_line = ft_sprintf("\nminishell : line %d: `%s'\n",
		shell->line_nb, shell->line[0]);
	}
	if (!shell->error_beg || !shell->error_line)
		exit_error(shell, 0);
}

void		free_line(t_shell *shell)
{
	ft_freez((void **)&shell->output);
	ft_lstclear(&shell->pipeline, free_cmd);
	ft_freez((void **)&shell->line[0]);
	ft_freez((void **)&shell->error_beg);
	ft_freez((void **)&shell->error_line);
}

void		free_cmd(void *content)
{
	t_cmd *cmd;

	cmd = (t_cmd*)content;
	if (cmd->fd_output > 2)
		close(cmd->fd_output);
	if (cmd->fd_input > 2)
		close(cmd->fd_input);
	ft_free_strs(&cmd->arg);
	free(cmd);
}

void		free_all(t_shell *shell)
{
	int			i;

	i = 0;
	while (shell->env_keys && shell->env_items && shell->env_keys[i])
	{
		ft_freez((void **)&shell->env_keys[i]);
		ft_freez((void **)&shell->env_items[i]);
		i++;
	}
	ft_freez((void **)&shell->env_keys);
	ft_freez((void **)&shell->env_items);
	ft_freez((void **)&shell->pwd);
	ft_freez((void **)&shell->oldpwd);
	ft_freez((void**)&shell->lastarg);
	free_line(shell);
}

void		close_all(t_shell *shell)
{
	if (shell->fd_output > 2)
		close(shell->fd_output);
	if (shell->fd_input > 2)
		close(shell->fd_input);
	if (shell->fd_line > 2)
	{
		close(shell->fd_line);
		close(shell->fd_line);
	}
}
