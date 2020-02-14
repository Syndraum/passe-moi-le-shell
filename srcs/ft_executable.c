/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executable.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roalvare <roalvare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/02 13:35:47 by mchardin          #+#    #+#             */
/*   Updated: 2020/02/14 11:43:40 by roalvare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		count_malloc_env(char **keys, char **items)
{
	int		i;
	int		count;

	count = 0;
	i = -1;
	while (keys[++i])
		count += !(items[i] == 0);
	return (count);
}

char	**convert_env(t_shell *shell)
{
	int		len;
	int		i;
	int		j;
	char	**env;

	i = 0;
	j = 0;
	len = count_malloc_env(shell->env_keys, shell->env_items);
	env = malloc(sizeof(char*) * (len + 1));
	while (shell->env_keys[i])
	{
		if (shell->env_items[i])
		{
			env[j] = ft_sprintf("%s=%s",
				shell->env_keys[i], shell->env_items[i]);
			j++;
		}
		i++;
	}
	env[j] = 0;
	return (env);
}

int		executable(t_shell *shell)
{
	char		**tab;
	pid_t		child;
	char		**env;
	struct stat	sb;

	env = convert_env(shell);
	tab = ft_split(get_item("PATH", shell->env_keys, shell->env_items), ':');
	ft_free_strs(tab);
	if (-1 == stat(shell->tab[0], &sb))
		return (127);
	child = fork();
	if (child == 0)
	{
		if (0 > execve(shell->tab[0], shell->tab, env))
		{
			shell->output = strerror(errno);
			ft_free_strs(env);
			return (127);
		}
	}
	else
		wait(&child);
	ft_free_strs(env);
	return (0);
}
