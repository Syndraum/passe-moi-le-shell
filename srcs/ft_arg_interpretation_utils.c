/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arg_interpretation_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 17:06:53 by mchardin          #+#    #+#             */
/*   Updated: 2020/04/22 21:52:23 by mchardin         ###   ########.fr       */
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

void	init_struct(t_shell *shell)
{
	shell->output = 0;
	shell->fd_output = 1;
}

int		init_tab(t_shell *shell)
{
	shell->fd_input = 0;
	shell->fd_output = 1;
	if (!(set_arg(shell)))
	{
		shell->tab = 0;
		return (0);
	}
	if (!(shell->tab = malloc(sizeof(char*) * 2)))
		exit_error(shell, shell->arg.str);
	shell->tab[0] = shell->arg.str;
	shell->tab[1] = NULL;
	return (1);
}
