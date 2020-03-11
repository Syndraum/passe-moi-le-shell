/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 17:19:59 by mchardin          #+#    #+#             */
/*   Updated: 2020/03/11 18:16:40 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		*home_path(char **keys, char **items)
{
	int		i;

	i = 0;
	while (keys[i])
	{
		if (!ft_strncmp("HOME", keys[i], 5))
			return (items[i]);
		i++;
	}
	ft_dprintf(2, "%scd: %s\n", ERR_HOME);
	return (0);
}

int			cd_path(t_shell *shell, char *dir)
{
	char		**cd_paths;
	int			i;
	char		*new;

	i = -1;
	cd_paths = 0;
	while (shell->env_keys[++i])
	{
		if (!ft_strncmp("CDPATH", shell->env_keys[i], 7) && shell->env_items[i]
		&& !(cd_paths = ft_split(shell->env_items[i], ':')))
			exit_error(shell, "cd");
	}
	i = -1;
	if (cd_paths)
	{
		while (cd_paths[++i])
		{
			new = cd_paths[i][ft_strlen(cd_paths[i]) - 1] == '/' ?
			ft_sprintf("%s%s", cd_paths[i], dir)
			: ft_sprintf("%s/%s", cd_paths[i], dir);
			if (!new)
			{
				ft_free_strs(&cd_paths);
				exit_error(shell, "cd");
			}
			if (chdir(new) >= 0)
			{
				ft_free_strs(&cd_paths);
				shell->output = ft_strjoin_gnl(new, "\n");
				return (1);
			}
			ft_freez((void **)&new);
		}
		ft_free_strs(&cd_paths);
	}
	return (0);
}

char		*print_export(char **keys, char **items)
{
	int		i;
	char	*output;
	char	*tmp;

	i = 0;
	output = 0;
	while (keys[i])
	{
		if (items[i])
			tmp = ft_sprintf("declare -x %s=\"%s\"\n", keys[i], items[i]);
		else
			tmp = ft_sprintf("declare -x %s\n", keys[i]);
		if (!tmp)
			return (0);
		output = ft_strjoin_gnl(output, tmp);
		ft_freez((void **)&tmp);
		i++;
	}
	return (output);
}
