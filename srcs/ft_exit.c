/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_quit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 15:05:53 by mchardin          #+#    #+#             */
/*   Updated: 2020/03/04 17:00:48 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
