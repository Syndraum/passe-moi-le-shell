/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arg_translation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roalvare <roalvare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 11:19:52 by roalvare          #+#    #+#             */
/*   Updated: 2020/03/05 17:41:51 by roalvare         ###   ########.fr       */
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

char	*get_dollar(char **cursor, t_shell *shell, char stop_char)
{
	char	*arg;
	char	*var;
	int		len;

	var = NULL;
	arg = NULL;
	len = 0;
	(*cursor)++;
	len = is_var_ret_idx(*cursor);
	if (is_stoparg(**cursor) || stop_char == **cursor || **cursor == '\\')
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
	ft_freez((void **)&arg);
	var = var ? ft_strdup(var) : ft_strdup("");
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
	(*cursor) += len;
	if (**cursor == '\'')
		(*cursor)++;
	return (quote);
}

void	*get_tilde(char **cursor, t_shell *shell)
{
	char	*arg;
	char	*home;

	if (*((*cursor) + 1) == '/' || is_stoparg(*((*cursor) + 1)))
	{
		home = get_item("HOME", shell->env_keys, shell->env_items);
		if (home == NULL)
			arg = strdup("");
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
	char *ret;

	arg = NULL;
	while (!is_stoparg(**cursor))
	{
		if (**cursor == '\'')
			ret = get_quote(cursor);
		else if (**cursor == '"')
			ret = get_dquote(cursor, shell);
		else if (**cursor == '~' && arg == NULL)
			ret = get_tilde(cursor, shell);
		else
			ret = get_unquote(cursor, shell);
		if (!ret)
		{
			free(arg);
			return (NULL);
		}
		arg = ft_strjoin_gnl(arg, ret);
		ft_freez((void **)&ret);
		if (!arg)
			return (NULL);
	}
	return (arg);
}

void	*set_arg(t_shell *shell)
{
	char	*arg;
	char	**cursor;

	cursor = shell->cursor;
	*cursor = skip_if(*cursor, ft_iswhitespace);
	if (**cursor != 0)
	{
		if (!(arg = get_argument(cursor, shell)))
			exit_error(shell, 0);
	}
	else
		arg = NULL;
	shell->arg.str = arg;
	shell->arg.sep = get_sep(cursor);
	return (arg);
}
