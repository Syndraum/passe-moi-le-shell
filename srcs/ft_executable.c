/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executable.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roalvare <roalvare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/02 13:35:47 by mchardin          #+#    #+#             */
/*   Updated: 2020/02/14 18:27:30 by roalvare         ###   ########.fr       */
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

char	*getpath(t_shell *shell)
{
	char			**tab;
	DIR				*dir;
	struct dirent	*entry;
	int				i;
	int				len;

	i = -1;
	len = ft_strlen(shell->tab[0]) + 1;
	tab = ft_split(get_item("PATH", shell->env_keys, shell->env_items), ':');
	while (tab[++i])
	{
		if ((dir = opendir(tab[i])) != NULL)
		{
			while ((entry = readdir(dir)) != NULL)
				if (ft_strncmp(entry->d_name, shell->tab[0], len) == 0)
					return (ft_strjoin_gnl(ft_strjoin(tab[i], "/"), shell->tab[0]));
			closedir(dir);
		}
	}
	return (NULL);
}

int		executable(t_shell *shell)
{
	pid_t		child;
	char		**env;
	char		*path;
	struct stat	sb;

	env = convert_env(shell);
	if ((path = getpath(shell)) == NULL)
	{
		path = shell->tab[0];
		if (-1 == stat(path, &sb))
			return (127);
	}
	child = fork();
	if (child == 0)
	{
		if (0 > execve(path, shell->tab, env))
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
