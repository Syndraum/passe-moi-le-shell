/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 21:25:23 by mchardin          #+#    #+#             */
/*   Updated: 2020/02/13 18:50:16 by mchardin         ###   ########.fr       */
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
			ft_printf("\nTEST var: %s\n\n", var);
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

int			pwd_env(t_shell *shell)
{
	int		i;
	char	*oldpwd;

	i = 0;
	while (shell->environ[i] && !is_var_to_unset("PWD", shell->environ[i]))
		i++;
	if (shell->environ[i])
	{
		if (!(oldpwd = ft_strjoin("OLD", shell->environ[i])))
			return (0);
		// if (!replace_or_add(&shell->environ, oldpwd))
		// 	return (0);
		free(shell->environ[i]);
		shell->environ[i] = ft_strjoin("PWD=", shell->pwd);
	}
	return (0);
}

int			last_arg_env(char ***environ, char **tab)
{
	int		i;
	char	*buf;

	i = ft_strslen(tab);
	buf = 0;
	(void)environ;
	// if (!(buf = ft_strjoin("_=", tab[i - 1])) ||
	// 	!replace_or_add(environ, buf))
	// 	return (0);
	return (1);
}
