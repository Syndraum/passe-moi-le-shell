/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 15:25:03 by mchardin          #+#    #+#             */
/*   Updated: 2020/04/14 14:59:47 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			ft_env_lib(t_shell *shell, char **env)
{
	int		i;
	char	*key;
	char	*item;
	int		ret;

	i = 0;
	if (!(shell->env_keys = malloc(sizeof(char*) * (ft_strslen(env) + 1)))
	|| !(shell->env_items = malloc(sizeof(char*) * (ft_strslen(env) + 1))))
		exit_error(shell, 0);
	while (env[i])
	{
		if ((ret = check_split_var(env[i], &key, &item)) < 0)
			exit_error(shell, 0);
		shell->env_keys[i] = key;
		shell->env_items[i] = item;
		if (!ft_strncmp(key, "_", 2) && !(shell->lastarg = ft_strdup(item)))
			exit_error(shell, 0);
		i++;
	}
	shell->env_keys[i] = 0;
	shell->env_items[i] = 0;
	return (1);
}

int			replace_item(char **keys, char **items, char *key, char *item)
{
	int		idx;

	idx = get_tabidx(key, keys);
	if (idx >= 0)
	{
		ft_freez((void **)&items[idx]);
		items[idx] = item;
		return (1);
	}
	return (0);
}

int			init_oldpwd(char ***keys, char ***items)
{
	int		end;
	char	*tmp;

	end = ft_strslen(*keys);
	if (get_tabidx("OLDPWD", *keys) < 0 &&
	(!(tmp = ft_strdup("OLDPWD"))
	|| !(*items = ft_strs_add_end(*items, 0, end))
	|| !(*keys = ft_strs_plus_one(*keys, tmp))))
		return (0);
	return (1);
}
