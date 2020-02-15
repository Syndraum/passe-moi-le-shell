/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_lib.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 15:25:03 by mchardin          #+#    #+#             */
/*   Updated: 2020/02/15 17:36:26 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			ft_env_lib(t_shell *shell, char **env)
{
	int		i;
	char	*key;
	char	*item;

	i = 0;
	if (!(shell->env_keys = malloc(sizeof(char*) * (ft_strslen(env) + 1)))
	|| !(shell->env_items = malloc(sizeof(char*) * (ft_strslen(env) + 1))))
		return (0); // error
	while (env[i])
	{
		check_split_var(env[i], &key, &item);
		shell->env_keys[i] = key;
		shell->env_items[i] = item;
		i++;
	}
	shell->env_keys[i] = 0;
	shell->env_items[i] = 0;
	return (1);
}

int			change_item(char **keys, char **items, char *key, char *item)
{
	int		idx;

	idx = get_tabidx(key, keys);
	if (idx >= 0)
	{
		free(items[idx]);
		items[idx] = item;
		return (1);
	}
	return (0);
}

int			init_oldpwd(char ***keys, char ***items)
{
	int		end;

	end = ft_strslen(*keys);
	if (!change_item(*keys, *items, "OLDPWD", 0) &&
	(!(*items = ft_strs_add_end(*items, 0, end))
	|| !(*keys = ft_strs_plus_one(*keys, ft_strdup("OLDPWD")))))
		return (0); // malooc error
	return (1);
}
