/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arg_translation_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roalvare <roalvare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 11:19:52 by roalvare          #+#    #+#             */
/*   Updated: 2020/02/14 14:45:34 by roalvare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_stoparg(char c)
{
	if (c == '\0' || c == ';' || c == '<' ||
	c == '>' || c == '|' || c == ' ' || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

int		is_stop_unquote(char c)
{
	if ((c == '$' || c == '\'' || c == '"' || is_stoparg(c)))
		return (1);
	return (0);
}

int		is_stop_dquote(char c)
{
	if ((c == '"' || c == '$' || is_stoparg(c)))
		return (1);
	return (0);
}

int		strlen_to(char *str, char stop)
{
	int i;

	i = 0;
	while (str[i] && str[i] != stop)
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
