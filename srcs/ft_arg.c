/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arg.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roalvare <roalvare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 11:19:52 by roalvare          #+#    #+#             */
/*   Updated: 2020/01/10 20:56:43 by roalvare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_whitespace(char c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

int		is_stoparg(char c)
{
	if (c == '\0' || c == ';' || c == '<' ||
	c == '>' || c == '|' || c == ' ' || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

int		strlen_to(char *str, char c)
{
	int i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
}

int		strlen_if(char *str, int (*f)(char))
{
	int i;

	i = 0;
	while (str[i] && !f(str[i]))
		i++;
	return (i);
}

void	*set_arg(t_shell *shell)
{
	char	*arg;
	int		len;
	char	**cursor;

	cursor = shell->cursor;
	*cursor = skip_if(*cursor, is_whitespace);
	if (**cursor == '\'')
	{
		(*cursor)++;
		len = strlen_to(*cursor, '\'');
	}
	else if (**cursor == '"')
	{
		(*cursor)++;
		len = strlen_to(*cursor, '"');
	}
	else
		len = strlen_if(*cursor, is_stoparg);
	arg = ft_calloc(len + 1, sizeof(char));
	ft_strlcpy(arg, *cursor, len + 1);
	*cursor += len;
	if (**cursor == '\'' || **cursor == '"')
		(*cursor)++;
	shell->arg.str = arg;
	shell->arg.sep = get_arg(cursor);
	ft_printf("sep = %d\n", shell->arg.sep);
	return (arg);
}
