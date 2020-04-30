/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 15:05:53 by mchardin          #+#    #+#             */
/*   Updated: 2020/04/30 12:24:59 by user42           ###   ########.fr       */
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
	exit(shell->stop);
}

void		print_error(t_shell *shell, char *s2, char *s1)
{
	ft_dprintf(2, "%s%s: %s%s", shell->error_beg, s2, s1, shell->error_line);
}
