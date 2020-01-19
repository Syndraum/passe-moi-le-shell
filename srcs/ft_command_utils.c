/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 21:25:23 by mchardin          #+#    #+#             */
/*   Updated: 2020/01/19 21:27:57 by mchardin         ###   ########.fr       */
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
