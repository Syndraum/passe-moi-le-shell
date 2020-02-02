/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arg_interpretation.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 19:48:21 by mchardin          #+#    #+#             */
/*   Updated: 2020/02/02 17:36:26 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_end_of_command(t_separator sep)
{
	if (sep == END_LINE || sep == SEMICOLON || sep == PIPE)
		return (1);
	return (0);
}

int		is_redirection(t_separator sep)
{
	if (sep == TO_END || sep == TO_FILE || sep == FROM_FILE)
		return (1);
	return (0);
}

int		init_struct(t_shell *shell)
{
	shell->output = 0;
	shell->fd = 1;
	if (!(set_arg(shell)))
	{
		shell->tab = 0;
		return (0); // EXIT ? RETURN ? 
	}
	if (!(shell->tab = malloc(sizeof(char*) * 2)))
	{
		free(shell->arg.str);
		exit(1);
	}
	shell->tab[0] = shell->arg.str;
	shell->tab[1] = NULL;
	return (1);
}

int		ft_redirection(t_shell *shell, t_separator prev)
{
	if (prev == FROM_FILE)
		return (1);
	if (shell->fd != 1)
		close(shell->fd);
	if (prev == TO_FILE && (shell->fd = open(shell->arg.str,
		O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU)) < 0)
	{
		ft_printf("minishell: %s: %s\n", shell->arg.str, strerror(errno));
		return (0); //NOT EXIT
	}
	else if (prev == TO_END && (shell->fd = open(shell->arg.str,
		O_CREAT | O_WRONLY | O_APPEND)) < 0)
	{
		ft_printf("minishell: %s: %s\n", shell->arg.str, strerror(errno));
		return (0); //NOT EXIT
	}
	free(shell->arg.str);
	return (1);
}

int		analyse_args(t_shell *shell)
{
	t_separator prev;

	if (!(init_struct(shell)))
		return (0);
	while (!is_end_of_command(shell->arg.sep))
	{
		prev = shell->arg.sep;
		if (!(set_arg(shell)))
		{
			ft_free_strs(shell->tab);
			return (0);
		}
		if (prev == ARG &&
		!(shell->tab = ft_strs_plus_one(shell->tab, shell->arg.str)))
			return (0);
		else if (is_redirection(prev) && !(ft_redirection(shell, prev)))
			return (0);
	}
	return (1);
}
