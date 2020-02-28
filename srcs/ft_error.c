/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 15:05:53 by mchardin          #+#    #+#             */
/*   Updated: 2020/02/28 20:01:19 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		free_all(t_shell *shell)
{
	int			i;

	i = 0;
	while (shell->env_keys[i])
	{
		ft_freez(shell->env_keys[i]);
		ft_freez(shell->env_items[i]);
		i++;
	}
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

}

void        quit_error(t_shell *shell, int errnumb, char *fonction, char *arg)
{
	if (errnumb > 0)
	{
		if (fonction)
			ft_dprintf(2, "minishell: %s: ", fonction);
		if (arg)
			ft_dprintf(2, "%s: ", arg);
		ft_dprintf(2, "", ft_strerror(errnumb));
	}
	// ft_free_strs(shell->tab);
	close_all(shell);
	free_all(shell);
	exit(1);
}
