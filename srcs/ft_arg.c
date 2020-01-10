/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arg.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roalvare <roalvare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 11:19:52 by roalvare          #+#    #+#             */
/*   Updated: 2020/01/10 17:53:34 by roalvare         ###   ########.fr       */
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

char	*get_argument(char **str)
{
	char	*arg;
	int		len;

	*str = skip_if(*str, is_whitespace);
	if (**str == '\'')
	{
		(*str)++;
		len = strlen_to(*str, '\'');
	}
	else if (**str == '"')
	{
		(*str)++;
		len = strlen_to(*str, '"');
	}
	else
		len = strlen_if(*str, is_stoparg);
	arg = ft_calloc(len + 1, sizeof(char));
	ft_strlcpy(arg, *str, len + 1);
	*str += len;
	if (**str == '\'' || **str == '"')
		(*str)++;
	return (arg);
}
