/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 15:05:53 by mchardin          #+#    #+#             */
/*   Updated: 2020/03/02 18:53:37 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		free_line(t_shell *shell)
{
	ft_freez((void **)&shell->output);
	ft_lstclear(&shell->pipeline, free_cmd);
	// ft_freez((void **)&shell->line);
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

void		exit_error(t_shell *shell, char *fonction)
{
	ft_dprintf(2, "minishell: ");
	if (fonction)
		ft_dprintf(2, "%s: ", fonction);
	ft_dprintf(2, "%s\n", strerror(errno));
	close_all(shell);
	free_all(shell);
	exit(1);
}

void		exit_end(t_shell *shell)
{
	ft_putstr_fd("exit\n", shell->fd_line);
	close_all(shell);
	free_all(shell);
	exit(0);
}
