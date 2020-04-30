/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 15:05:53 by mchardin          #+#    #+#             */
/*   Updated: 2020/04/30 13:49:40 by user42           ###   ########.fr       */
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

void		to_exit(t_shell *shell)
{
	close_all(shell);
	free_all(shell);
	exit(shell->stop);
}

void		exit_end(t_shell *shell)
{
	int len;
	int error;

	ft_putstr_fd("exit\n", shell->fd_line);
	if ((len = ft_strslen(shell->tab)) > 1)
	{
		if (!(error = ft_atoi(shell->tab[1])))
		{
			shell->stop = 2;
			print_error(shell, "exit ", "numeric argument needed");
			to_exit(shell);
		}
		else if (len > 2)
		{
			print_error(shell, "exit", "too many argument");
			shell->stop = 1;
		}
		else
		{
			shell->stop = error;
			to_exit(shell);
		}
	}
	else
		to_exit(shell);
}

void		print_error(t_shell *shell, char *s2, char *s1)
{
	ft_dprintf(2, "%s%s: %s%s", shell->error_beg, s2, s1, shell->error_line);
}
