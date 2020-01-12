/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arg.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roalvare <roalvare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 11:19:52 by roalvare          #+#    #+#             */
/*   Updated: 2020/01/12 14:13:15 by roalvare         ###   ########.fr       */
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

char	*get_quote(char **cursor)
{
	char	*quote;
	int		len;

	(*cursor)++;
	len = strlen_to(*cursor, '\'');
	if (!(quote = ft_calloc(len + 1, sizeof(char))))
		return (NULL);
	ft_strlcpy(quote, *cursor, len + 1);
	(*cursor) += len + 1;
	return (quote);
}

char	*get_dquote(char **cursor)
{
	char	*bquote;
	int		len;

	(*cursor)++;
	len = strlen_to(*cursor, '"');
	if (!(bquote = ft_calloc(len + 1, sizeof(char))))
		return (NULL);
	ft_strlcpy(bquote, *cursor, len + 1);
	(*cursor) += len + 1;
	return (bquote);
}

char	*get_noquote(char **cursor)
{
	char	*arg;
	int		len;

	len = 0;
	while ((*cursor)[len] != '\'' && (*cursor)[len] != '"' && !is_stoparg((*cursor)[len]))
		len++;
	if (!(arg = ft_calloc(len + 1, sizeof(char))))
		return (NULL);
	ft_strlcpy(arg, *cursor, len + 1);
	(*cursor) += len;
	return (arg);
}

void	*get_argument(char	**cursor)
{
	char *arg;
	char *tmp;
	char *ret;

	arg = NULL;
	tmp = NULL;
	ret = NULL;
	while (!is_stoparg(**cursor))
	{
		if (**cursor == '\'')
		{
			if (!(ret = get_quote(cursor)))
				return (NULL);
		}
		else if (**cursor == '"')
		{
			if (!(ret = get_dquote(cursor)))
				return (NULL);
		}
		else
		{
			if (!(ret = get_noquote(cursor)))
				return (NULL);
		}
		if (arg != NULL)
			tmp = ft_strdup(arg);
		if (arg != NULL)
			free(arg);
		arg = NULL;
		if (!(arg = ft_strjoin(tmp, ret)))
			return (NULL);
		free(tmp);
		tmp = NULL;
		free(ret);
		ret = NULL;
	}
	return (arg);
}

void	*set_arg(t_shell *shell)
{
	char	*arg;
	char	**cursor;

	cursor = shell->cursor;
	*cursor = skip_if(*cursor, is_whitespace);
	arg = get_argument(cursor);
	shell->arg.str = arg;
	shell->arg.sep = get_arg(cursor);
	return (arg);
}
