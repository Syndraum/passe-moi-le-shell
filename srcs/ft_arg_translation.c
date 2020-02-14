/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arg_translation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 11:19:52 by roalvare          #+#    #+#             */
/*   Updated: 2020/02/14 10:58:15 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		get_tabidx(char *key, char **keys)
{
	int i;
	int len;

	i = -1;
	len = ft_strlen(key);
	while (keys[++i] != 0)
	{
		if (!ft_strncmp(key, keys[i], len + 1))
			return (i);
	}
	return (-1);
}

char	*get_item(char *key, char **keys, char **items)
{
	char	*value;
	int		index;

	value = NULL;
	if ((index = get_tabidx(key, keys)) == -1)
		return (value);
	return (items[index]);
}

char	*get_dollar(char **cursor, t_shell *shell)
{
	char	*arg;
	char	*var;
	int		len;

	var = NULL;
	arg = NULL;
	len = 0;
	(*cursor)++;
	len = is_var_ret_idx(*cursor);
	if (is_stoparg(**cursor))
		return (ft_strdup("$"));
	if (**cursor == '?')
	{
		(*cursor)++;
		return (ft_itoa(shell->stop));
	}
	if (!(arg = ft_calloc(len + 1, sizeof(char))))
		return (NULL);
	ft_strlcpy(arg, *cursor, len + 1);
	(*cursor) += len;
	var = get_item(arg, shell->env_keys, shell->env_items);
	if (var)
		var = ft_strdup(var);
	free(arg);
	return (var);
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

char	*get_dquote(char **cursor, t_shell *shell)
{
	char	*bquote;
	char	*tmp;
	int		i;

	bquote = NULL;
	tmp = NULL;
	i = 0;
	(*cursor)++;
	while ((*cursor)[i] != '"')
	{
		if ((*cursor)[i] == '$')
		{
			if (!(tmp = ft_calloc(i + 1, sizeof(char))))
				return (NULL);
			ft_strlcpy(tmp, *cursor, i + 1);
			(*cursor) += i;
			bquote = ft_strjoin_gnl(bquote, tmp);
			bquote = ft_strjoin_gnl(bquote, get_dollar(cursor, shell));
			free(tmp);
			i = 0;
		}
		else
			i++;
	}
	if (!(tmp = ft_calloc(i + 1, sizeof(char))))
		return (NULL);
	ft_strlcpy(tmp, *cursor, i + 1);
	bquote = ft_strjoin_gnl(bquote, tmp);
	free(tmp);
	(*cursor) += i + 1;
	return (bquote);
}

char	*get_noquote(char **cursor, t_shell *shell)
{
	char	*arg;
	char	*tmp;
	int		len;

	len = 0;
	arg = NULL;
	while (!is_stop_noquote((*cursor)[len]))
		len++;
	if (!(arg = ft_calloc(len + 1, sizeof(char))))
		return (NULL);
	ft_strlcpy(arg, *cursor, len + 1);
	(*cursor) += len;
	if (**cursor == '$')
	{
		tmp = ft_strjoin(arg, get_dollar(cursor, shell));
		free(arg);
		arg = tmp;
	}
	return (arg);
}

void	*get_argument(char **cursor, t_shell *shell)
{
	char *arg;
	char *cpy;
	char *ret;

	arg = NULL;
	cpy = NULL;
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
			if (!(ret = get_dquote(cursor, shell)))
				return (NULL);
		}
		else
		{
			if (!(ret = get_noquote(cursor, shell)))
				return (NULL);
		}
		if (arg != NULL)
			cpy = ft_strdup(arg);
		if (arg != NULL)
			free(arg);
		arg = NULL;
		if (!(arg = ft_strjoin(cpy, ret)))
			return (NULL);
		free(cpy);
		cpy = NULL;
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
	*cursor = skip_if(*cursor, ft_iswhitespace);
	arg = get_argument(cursor, shell);
	shell->arg.str = arg;
	shell->arg.sep = get_arg(cursor);
	return (arg);
}
