/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 21:25:23 by mchardin          #+#    #+#             */
/*   Updated: 2020/03/02 21:32:05 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				check_split_var(char *var, char **key, char **item) // coupe var en key et item
{
	int		idx;

	*key = 0;
	*item = 0;
	if (!(idx = is_var_ret_idx(var)))
		return (1); // "not a valid identifer"
	if (idx == (int)ft_strlen(var))
	{
		if (!(*key = ft_strdup(var)))
			return (-1); //error
	}
	else if (var[idx] == '=')
	{
		if (!(*key = ft_strndup(var, strlen_to(var, '=')))
		|| !(*item = ft_strdup(ft_strchr(var, '=') + 1)))
			return (-1); //error
	}
	else
		return (1);
	return (0); //went well 
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
				if(!(items[idx] = ft_strdup("")))
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
			// ft_printf("key =  %s\n", key);
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
			return (0); //malloc error
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
			exit_error(shell, "cd"); // free old
	}
}

int				last_arg_env(char ***keys, char ***items, char **tab)
{
	int		i;
	char	*key;
	char	*item;

	i = ft_strslen(tab);
	if (!(key = ft_strdup("_"))
	|| !(item = ft_strdup(tab[i - 1]))
	|| !replace_or_add(keys, items, key, item))
		return (0);
	return (1);
}

// int			replace_or_add_key(char ***keys, char ***items, char *var)
// {
// 	char	*new_key;
// 	char	*new_item;

// 	new_item = 0;
// 	if (is_var_ret_idx(var) == ft_strlen(var) &&
// 	!(new_key = ft_strdup(var)))
// 		return (0); //error
// 	else if (!replace_var(*keys, *items, var) &&
// 	(!(new_key = ft_strndup(var, strlen_to(var, '=')))
// 		|| !(new_item = ft_strdup(ft_strchr(var, '=') + 1))))
// 			return (0); //error
// 	else
// 		return (1);
// 		|| !(*keys = ft_strs_plus_one(*keys, new_key))
// 		|| !(*items = ft_strs_plus_one(*items, new_item))))
// 	return (1);
// }

// int				replace_var(char **keys, char **items, char *key, char *item)
// {
// 	int		i;

// 	i = 0;
// 	while (keys[i])
// 	{
// 		if (!ft_strncmp(keys[i], key, ft_strlen(key) + 1))
// 		{
// 			ft_freez((void **)&items[i]);
// 			items[i] = item;
// 			return (1);
// 		}
// 		i++;
// 	}
// 	return (0);
// }
