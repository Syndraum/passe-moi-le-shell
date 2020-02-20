/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arg_translation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roalvare <roalvare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 11:19:52 by roalvare          #+#    #+#             */
/*   Updated: 2020/02/20 11:44:44 by roalvare         ###   ########.fr       */
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
	int		index;

	if ((index = get_tabidx(key, keys)) == -1)
		return (0);
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

char	*get_tmp_quote(char **cursor, char *bquote, int i)
{
	char	*tmp;
	char	*quote;

	tmp = NULL;
	quote = NULL;
	if (!(tmp = ft_calloc(i + 1, sizeof(char))))
		return (NULL);
	ft_strlcpy(tmp, *cursor, i + 1);
	(*cursor) += i;
	quote = ft_strjoin_gnl(bquote, tmp);
	free(tmp);
	return (quote);
}

char	*get_dquote(char **cursor, t_shell *shell)
{
	char	*bquote;
	int		i;

	bquote = NULL;
	i = 0;
	(*cursor)++;
	while ((*cursor)[i] != '"' && !is_stoparg((*cursor)[i]))
	{
		if ((*cursor)[i] == '$')
		{
			bquote = get_tmp_quote(cursor, bquote, i);
			bquote = ft_strjoin_gnl(bquote, get_dollar(cursor, shell));
			i = 0;
		}
		else
			i++;
	}
	bquote = get_tmp_quote(cursor, bquote, i);
	(*cursor)++;
	return (bquote);
}

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
		if (**cursor == '\\')
			i++;
		len++;
		i++;
	}
	if (!(arg = ft_calloc(len + 1, sizeof(char))))
		return (NULL);
	strncmp_esc(arg, *cursor, i);
	(*cursor) += i;
	if (**cursor == '$')
		arg = ft_strjoin_gnl(arg, get_dollar(cursor, shell));
	return (arg);
}

void	*get_tilde(char **cursor, t_shell *shell)
{
	char	*arg;
	char	*home;

	if (*((*cursor) + 1) == '/' || is_stoparg(*((*cursor) + 1)))
	{
		home = get_item("HOME", shell->env_keys, shell->env_items);
		if (home == NULL)
			arg = (strdup(""));
		else
			arg = strdup(home);
	}
	else
		arg = strdup("~");
	(*cursor)++;
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
		else if (**cursor == '~' && arg == NULL)
			ret = get_tilde(cursor, shell);
		else
		{
			if (!(ret = get_unquote(cursor, shell)))
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
