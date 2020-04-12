/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 21:25:23 by mchardin          #+#    #+#             */
/*   Updated: 2020/03/08 18:50:46 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				check_split_var(char *var, char **key, char **item)
{
	int		idx;

	*key = 0;
	*item = 0;
	if (!(idx = is_var_ret_idx(var)))
		return (1);
	if (idx == (int)ft_strlen(var))
	{
		if (!(*key = ft_strdup(var)))
			return (-1);
	}
	else if (var[idx] == '=')
	{
		if (!(*key = ft_strndup(var, strlen_to(var, '=')))
		|| !(*item = ft_strdup(ft_strchr(var, '=') + 1)))
			return (-1);
	}
	else
		return (1);
	return (0); 
}

int			unset_var(char **keys, char **items, char *key)
{
	int		idx;
	int		last;

	last = ft_strslen(keys) - 1;
	idx = get_tabidx(key, keys);
	if (idx >= 0)
	{
		ft_freez((void **)&keys[idx]);
		ft_freez((void **)&items[idx]);
		keys[idx] = keys[last];
		items[idx] = items[last];
		keys[last] = 0;
		items[last] = 0;
		if (!ft_strncmp(key, "PWD", 4))
		{
			idx = get_tabidx("OLDPWD", keys);
			if (idx >= 0 && items[idx])
			{
				ft_freez((void **)&items[idx]);
				if (!(items[idx] = ft_strdup("")))
					return (0);
			}
		}
	}
	return (1);
}

int			replace_or_add(char ***keys, char ***items, char *key, char *item)
{
	int		idx;

	if (!key)
		return (0);
	idx = get_tabidx(key, *keys);
	if (idx >= 0)
	{
		if (item)
		{
			ft_freez((void **)&items[0][idx]);
			items[0][idx] = item;
		}
		ft_freez((void **)&key);
		return (1);
	}
	else
	{
		idx = ft_strslen(*keys);
		if (!(*keys = ft_strs_plus_one(*keys, key))
		|| !(*items = ft_strs_add_end(*items, item, idx)))
			return (0);
	}
	return (1);
}

void			pwd_env(t_shell *shell)
{
	int		idx;

	idx = get_tabidx("PWD", shell->env_keys);
	if (idx >= 0)
	{
		if (shell->env_items[idx])
		{
			ft_freez((void**)&shell->oldpwd);
			shell->oldpwd = shell->env_items[idx];
		}
		if (!(shell->env_items[idx] = ft_strdup(shell->pwd)))
			exit_error(shell, "cd");
	}
	idx = get_tabidx("OLDPWD", shell->env_keys);
	if (idx >= 0 && shell->oldpwd)
	{
		ft_freez((void **)&shell->env_items[idx]);
		if (!(shell->env_items[idx] = ft_strdup(shell->oldpwd)))
			exit_error(shell, "cd");
	}
}

char			*last_arg_env(t_shell *shell, char **tab)
{
	int		i;
	int		equal;
	char	*last_arg;

	ft_freez((void**)&shell->lastarg);
	if (!tab)
		return (0);
	i = ft_strslen(tab);
	equal = strlen_to(tab[i - 1], '=');
	if (equal == (int)ft_strlen(tab[i - 1]))
		last_arg = ft_strdup(tab[i - 1]);
	else if (i == 1)
		last_arg = ft_strdup("");
	else
		last_arg = ft_strndup(tab[i - 1], equal);
	if (!last_arg)
		exit_error(shell, tab[0]);
	return (last_arg);
}
