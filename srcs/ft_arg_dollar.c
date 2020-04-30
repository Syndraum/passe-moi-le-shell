/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arg_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/13 12:57:25 by mchardin          #+#    #+#             */
/*   Updated: 2020/04/30 15:09:46 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*get_var_env(char **cursor)
{
	char	*arg;
	int		len;

	arg = NULL;
	len = is_var_ret_idx(*cursor);
	if (!(arg = ft_calloc(len + 1, sizeof(char))))
		return (NULL);
	ft_strlcpy(arg, *cursor, len + 1);
	(*cursor) += len;
	return (arg);
}

char	*get_dollar(char **cursor, t_shell *shell, char stop_char)
{
	char	*arg;
	char	*var;

	var = NULL;
	arg = NULL;
	(*cursor)++;
	if (is_stoparg(**cursor) || stop_char == **cursor || **cursor == '\\')
		return (ft_strdup("$"));
	if (**cursor == '?')
	{
		(*cursor)++;
		return (ft_itoa(STOP));
	}
	else if (**cursor == '_')
	{
		(*cursor)++;
		return (ft_strdup(shell->lastarg));
	}
	arg = get_var_env(cursor);
	var = get_item(arg, shell->env_keys, shell->env_items);
	ft_freez((void **)&arg);
	var = var ? ft_strdup(var) : ft_strdup("");
	return (var);
}
