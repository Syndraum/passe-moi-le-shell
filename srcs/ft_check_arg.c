/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_arg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/20 20:20:05 by mchardin          #+#    #+#             */
/*   Updated: 2020/04/22 21:51:03 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		*get_separator(int sep)
{
	if (sep == TO_FILE)
		return (">");
	else if (sep == TO_END)
		return (">>");
	else if (sep == FROM_FILE)
		return ("<");
	else if (sep == SEMICOLON)
		return (";");
	else if (sep == PIPE)
		return ("|");
	return (NULL);
}

int			arg_empty(t_shell *shell, char **sep)
{
	if (!shell->arg.str && shell->arg.sep)
	{
		free(shell->arg.str);
		*sep = get_separator(shell->arg.sep);
		print_error(shell, ERR_TOKEN, *sep);
		return (1);
	}
	return (0);
}

int			check_arg(t_shell *shell)
{
	char	**cursor;
	char	*sep;

	cursor = shell->cursor2;
	shell->arg.sep = 0;
	while (**cursor)
	{
		*cursor = skip_if(*cursor, ft_iswhitespace);
		shell->arg.str = get_argument(cursor, shell);
		shell->arg.sep = get_sep(cursor);
		if (arg_empty(shell, &sep))
			return (0);
		ft_freez((void**)&shell->arg.str);
		*cursor = skip_if(*cursor, ft_iswhitespace);
	}
	if (!shell->arg.str && is_redirection(shell->arg.sep))
	{
		print_error(shell, ERR_TOKEN, "'mewline'");
		return (0);
	}
	return (1);
}
