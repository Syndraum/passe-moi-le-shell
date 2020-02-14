/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 21:25:23 by mchardin          #+#    #+#             */
/*   Updated: 2020/02/14 17:37:53 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				replace_var(char **keys, char **items, char *var)
{
	int		i;

	i = 0;
	while (keys[i])
	{
		if (is_key_var(keys[i], var))
		{
			free(items[i]);
			items[i] = ft_strdup(ft_strchr(var, '=') + 1);
			return (1);
		}
		i++;
	}
	return (0);
}

void			unset_var(char **keys, char **items, char *var)
{
	int		i;
	int		last;

	if (!is_var_ret_idx(var))
	{
		ft_dprintf(2, "minishell: unset: '%s': not a valid identifier\n", var);
		return ;
	}
	last = ft_strslen(keys) - 1;
	i = 0;
	while (keys[i])
	{
		if (!ft_strncmp(var, keys[i], ft_strlen(keys[i] + 1)))
		{
			free(keys[i]);
			free(items[i]);
			keys[i] = keys[last];
			items[i] = items[last];
			keys[last] = 0;
			items[last] = 0;
			return ;
		}
		i++;
	}
}

int			replace_or_add(char ***keys, char ***items, char *var)
{
	char	*new_key;
	char	*new_item;

	if (!replace_var(*keys, *items, var) &&
	(!(new_key = ft_strndup(var, strlen_to(var, '=')))
		|| !(new_item = ft_strdup(ft_strchr(var, '=') + 1))
		|| !(*keys = ft_strs_plus_one(*keys, new_key))
		|| !(*items = ft_strs_plus_one(*items, new_item))))
			return (0); //error
	return (1);
}

int				pwd_env(t_shell *shell)
{
	int		i;

	i = 0;
	while (shell->env_keys[i] && ft_strncmp("PWD", shell->env_keys[i], 4))
		i++;
	if (shell->env_keys[i])
	{
		shell->oldpwd = shell->env_items[i];
		shell->env_items[i] = ft_strdup(shell->pwd);
	}
	i = 0;
	while (shell->env_keys[i] && ft_strncmp("OLDPWD", shell->env_keys[i], 7))
		i++;
	if (shell->env_keys[i])
	{
		free(shell->env_items[i]);
		shell->env_items[i] = ft_strdup(shell->oldpwd); // free old
	}
	return (0);
}

int				last_arg_env(char ***keys, char ***items, char **tab)
{
	int		i;
	char	*buf;

	i = ft_strslen(tab);
	buf = 0;
	if (!(buf = ft_strjoin("_=", tab[i - 1])) ||
		!replace_or_add(keys, items, buf))
		return (0);
	return (1);
}
