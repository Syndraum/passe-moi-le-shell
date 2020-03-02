/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arg_translation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 11:19:52 by roalvare          #+#    #+#             */
/*   Updated: 2020/03/02 18:52:15 by mchardin         ###   ########.fr       */
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
	ft_freez((void **)&arg);
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
		return (NULL);
	strncmp_esc_dquote(tmp, *cursor, i);
	(*cursor) += i;
	quote = ft_strjoin_gnl(dquote, tmp);
	ft_freez((void **)&tmp);
	return (quote);
}

char	*get_dquote(char **cursor, t_shell *shell)
{
	char	*dquote;
	int		len;
	int		i;

	dquote = NULL;
	len = 0;
	i = 0;
	(*cursor)++;
	while ((*cursor)[i] != '"' && (*cursor)[i] != 0)
	{
		if ((*cursor)[i] == '$')
		{
			dquote = get_tmp_dquote(cursor, dquote, len, i);
			dquote = ft_strjoin_gnl(dquote, get_dollar(cursor, shell));
			i = 0;
			len = 0;
		}
		else
		{
			if ((*cursor)[i] == '\\' && (*cursor)[i + 1] == '"')
				i++;
			i++;
			len++;
		}
	}
	dquote = get_tmp_dquote(cursor, dquote, len, i);
	(*cursor)++;
	return (dquote);
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
			ft_freez((void **)&arg);
		arg = NULL;
		if (!(arg = ft_strjoin(cpy, ret)))
			return (NULL);
		ft_freez((void **)&cpy);
		cpy = NULL;
		ft_freez((void **)&ret);
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
