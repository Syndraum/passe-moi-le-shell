/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 15:05:53 by mchardin          #+#    #+#             */
/*   Updated: 2020/02/29 17:29:02 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		free_all(t_shell *shell)
{
	int			i;

	i = 0;
	while (shell->env_keys && shell->env_items && shell->env_keys[i])
	{
		ft_freez(shell->env_keys[i]);
		ft_freez(shell->env_items[i]);
		i++;
	}
	ft_freez(shell->arg.str);
	ft_freez(shell->env_keys);
	ft_freez(shell->env_items);
	ft_freez(shell->output);
	ft_freez(shell->pwd);
	ft_freez(shell->oldpwd);
	ft_lstclear(&shell->pipeline, free_cmd);
}

void		close_all(t_shell *shell)
{
	if (shell->fd_output > 2)
		close(shell->fd_output);
	if (shell->fd_input > 2)
		close(shell->fd_input);
	if (shell->fd_line > 2)
		close(shell->fd_line);
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
