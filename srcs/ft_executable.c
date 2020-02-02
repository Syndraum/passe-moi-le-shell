/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executable.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roalvare <roalvare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/02 13:35:47 by mchardin          #+#    #+#             */
/*   Updated: 2020/02/02 19:49:42 by roalvare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	executable(t_shell *shell)
{
	char		**tab;
	pid_t		child;
	struct stat	sb;

	tab = ft_split(get_tabvalue("PATH", shell->environ), ':');
	ft_free_strs(tab);
	if (-1 == stat(shell->tab[0], &sb))
		return (127);
	child = fork();
	if (child == 0)
	{
		if (-1 == execve(shell->tab[0], shell->tab, shell->environ))
		{
			shell->output = strerror(errno);
			return (127);
		}
	}
	else
		wait(&child);
	return (0);
}
