/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arg_unquote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roalvare <roalvare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 17:20:49 by roalvare          #+#    #+#             */
/*   Updated: 2020/03/05 17:21:15 by roalvare         ###   ########.fr       */
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

char	*get_dollar_unquote(char **cursor, t_shell *shell, char *arg)
{
	char	*dollar;

	if (!(dollar = get_dollar(cursor, shell, ' ')))
	{
		free(arg);
		return (NULL);
	}
	arg = ft_strjoin_gnl(arg, dollar);
	ft_freez((void**)&dollar);
	return (arg);
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
		if (**cursor == '\\')
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
