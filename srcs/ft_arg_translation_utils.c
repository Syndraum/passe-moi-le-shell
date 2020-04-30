/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arg_translation_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 11:19:52 by roalvare          #+#    #+#             */
/*   Updated: 2020/04/30 16:15:48 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_g_stoparg(char c)
{
	if (c == '\0' || c == ';' || c == '<' ||
	c == '>' || c == '|' || c == ' ' || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

int		is_g_stop_unquote(char c)
{
	if ((c == '$' || c == '\'' || c == '"' || is_g_stoparg(c)))
		return (1);
	return (0);
}

int		is_g_stop_dquote(char c)
{
	if ((c == '"' || c == '$' || is_g_stoparg(c)))
		return (1);
	return (0);
}

int		strlen_to(char *str, char g_stop)
{
	int i;

	i = 0;
	while (str[i] && str[i] != g_stop)
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
