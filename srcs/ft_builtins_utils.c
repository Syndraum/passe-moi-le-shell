/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 21:25:23 by mchardin          #+#    #+#             */
/*   Updated: 2020/02/02 16:30:34 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				replace_var(char **environ, char *var)
{
	int		i;

	i = 0;
	while (environ[i])
	{
		if (is_same_var(var, environ[i]))
		{
			free(environ[i]);
			environ[i] = var;
			return (1);
		}
		i++;
	}
	return (0);
}

void			unset_var(char **environ, char *var)
{
	int		i;
	int		last;

	if (!is_var_ret_idx(var))
	{
		ft_dprintf(2, "minishell: unset: '%s': not a valid identifier\n", var);
		return ;
	}
	last = ft_strslen(environ) - 1;
	i = 0;
	while (environ[i])
	{
		if (is_var_to_unset(var, environ[i]))
		{
			free(environ[i]);
			environ[i] = environ[last];
			environ[last] = 0;
			return ;
		}
		i++;
	}
}

int			replace_or_add(char ***environ, char *var)
{
	if (!replace_var(*environ, var) &&
		!(*environ = ft_strs_plus_one(*environ, var)))
		return (0);
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
		if (!replace_or_add(&shell->environ, oldpwd))
			return (0);
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
	if (!(buf = ft_strjoin("_=", tab[i - 1])) ||
		!replace_or_add(environ, buf))
		return (0);
	return (1);
}
