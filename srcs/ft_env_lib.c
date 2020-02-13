/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_lib.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 15:25:03 by mchardin          #+#    #+#             */
/*   Updated: 2020/02/13 17:49:59 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_env_lib(t_shell *shell, char **env)
{
	int		i;
	int		sep;

	i = 0;
	if (!(shell->env_keys = malloc(sizeof(char*) * (ft_strslen(env) + 1)))
	|| !(shell->env_items = malloc(sizeof(char*) * (ft_strslen(env) + 1))))
		return (0); // error
	while (env[i])
	{
		sep = strlen_to(env[i], '=');
		if (!(shell->env_keys[i] = ft_strndup(env[i], sep))
		|| !(shell->env_items[i] = ft_strdup(ft_strchr(env[i], '=') + 1)))
			return (0); // error
		i++;
	}
	shell->env_keys[i] = 0;
	shell->env_items[i] = 0;
	return (1);
}
