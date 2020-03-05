/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arg_dquote.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roalvare <roalvare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 17:09:04 by roalvare          #+#    #+#             */
/*   Updated: 2020/03/05 17:09:39 by roalvare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*strncmp_esc_dquote(char *dest, char *src, int len)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (src[i] && i < len)
	{
		if (src[i] == '\\' && src[i + 1] == '"')
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

char	*get_tmp_dquote(char **cursor, char *dquote, int len, int i)
{
	char	*tmp;
	char	*quote;

	tmp = NULL;
	quote = NULL;
	if (!(tmp = ft_calloc(len + 1, sizeof(char))))
	{
		free(dquote);
		return (NULL);
	}
	strncmp_esc_dquote(tmp, *cursor, i);
	(*cursor) += i;
	quote = ft_strjoin_gnl(dquote, tmp);
	ft_freez((void **)&tmp);
	return (quote);
}

char	*get_dollar_dquote(char **cursor, t_shell *shell, char *dquote)
{
	char	*dollar;

	if (!(dollar = get_dollar(cursor, shell, '"')))
	{
		free(dquote);
		return (NULL);
	}
	dquote = ft_strjoin_gnl(dquote, dollar);
	ft_freez((void**)&dollar);
	if (!dquote)
		return (NULL);
	return (dquote);
}

char	*init_dquote(char **cursor, int *len, int *i)
{
	*len = 0;
	*i = -1;
	(*cursor)++;
	return (NULL);
}

char	*get_dquote(char **cursor, t_shell *shell)
{
	char	*dquote;
	int		len;
	int		i;

	dquote = init_dquote(cursor, &len, &i);
	while ((*cursor)[++i] != '"' && (*cursor)[i] != 0)
	{
		if ((*cursor)[i] == '$')
		{
			if (!(dquote = get_tmp_dquote(cursor, dquote, len, i)))
				return (NULL);
			if (!(dquote = get_dollar_dquote(cursor, shell, dquote)))
				return (NULL);
			i = -1;
			len = -1;
		}
		else if ((*cursor)[i] == '\\' && (*cursor)[i + 1] == '"')
			i++;
		len++;
	}
	if (!(dquote = get_tmp_dquote(cursor, dquote, len, i)))
		return (NULL);
	if (**cursor == '"')
		(*cursor)++;
	return (dquote);
}