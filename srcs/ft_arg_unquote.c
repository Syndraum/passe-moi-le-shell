/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arg_unquote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 17:20:49 by roalvare          #+#    #+#             */
/*   Updated: 2020/04/22 21:50:38 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*strncmp_esc(char *dest, char *src, int len)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (src[i] && i < len)
	{
		if (src[i] == '\\')
			i++;
		if (src[i])
		{
			dest[j] = src[i];
			i++;
			j++;
		}
	}
	dest[j] = 0;
	return (dest);
}

char	*get_unquote(char **cursor, t_shell *shell)
{
	char	*arg;
	int		len;
	int		i;

	len = 0;
	i = 0;
	arg = NULL;
	while (!is_stop_unquote((*cursor)[i]))
	{
		if ((*cursor)[i] == '\\')
			i++;
		len++;
		i++;
	}
	if (!(arg = ft_calloc(len + 1, sizeof(char))))
		return (NULL);
	strncmp_esc(arg, *cursor, i);
	(*cursor) += i;
	if (**cursor == '$' && !(arg = get_dollar_unquote(cursor, shell, arg)))
		return (NULL);
	return (arg);
}
