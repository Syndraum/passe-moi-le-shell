/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_var.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 22:13:27 by mchardin          #+#    #+#             */
/*   Updated: 2020/02/13 17:26:43 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			is_var_ret_idx(char *var)
{
	int i;

	i = 0;
	if (!(ft_isalpha(var[i]) || var[i] == '_'))
		return (0);
	while (ft_isalnum(var[i]) || var[i] == '_')
		i++;
	return (i);
}

int			is_key_var(char *key, char *var)
{
	int		i;

	i = 0;
	while (key[i] && var[i] && key[i] == var[i])
		i++;
	if (var[i] != '=' || key[i] != 0)
		return (0);
	return (1);
}

int			is_var_to_unset(char *var, char *vardef)
{
	int		i;

	i = 0;
	while (var[i] && vardef[i])
	{
		if (var[i] == '=' || var[i] != vardef[i])
			return (0);
		i++;
	}
	if (vardef[i] != '=')
		return (0);
	return (1);
}

int			is_var_def(char *var)
{
	int i;

	if (!(i = is_var_ret_idx(var)))
	{
		ft_dprintf(2, "minishell: export: `%s': not a valid identifier\n", var);
		return (0);
	}
	if (var[i] != '=')
		return (0);
	return (1);
}
