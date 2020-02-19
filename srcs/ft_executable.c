/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executable.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roalvare <roalvare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/02 13:35:47 by mchardin          #+#    #+#             */
/*   Updated: 2020/02/19 16:24:38 by roalvare         ###   ########.fr       */
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

char	*try_path(char *filename, char *path_dir)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*find;
	int				len;

	len = ft_strlen(filename) + 1;
	if ((dir = opendir(path_dir)) != NULL)
	{
		while ((entry = readdir(dir)) != NULL)
		{
			if (ft_strncmp(entry->d_name, filename, len) == 0)
			{
				closedir(dir);
				if (!(find = ft_sprintf("%s%c%s", path_dir, '/', filename)))
					return (NULL);
				return (find);
			}
		}
		closedir(dir);
	}
	return (NULL);
}

char	*getpath(t_shell *shell)
{
	char			**tab;
	int				i;
	char			*path;
	struct stat		sb;

	i = -1;
	if (!(tab = ft_split(get_item("PATH", shell->env_keys, shell->env_items), ':')))
		return (NULL);
	while (tab[++i])
	{
		if ((path = try_path(shell->tab[0], tab[i])))
		{
			ft_free_strs(tab);
			return (path);
		}
	}
	ft_free_strs(tab);
	if (!(path = ft_strdup(shell->tab[0])))
		return (NULL);
	if (-1 == stat(path, &sb))
		return (NULL);
	return (path);
}

int	change_exec(t_shell *shell, char *path)
{
	char		**env;

	if (!(env = convert_env(shell)))
		return (127);
	if (0 > execve(path, shell->tab, env))
		return (127);
	return (0);
}

int	launch_exec(t_shell *shell, char *path)
{
	pid_t		child;

	if ((child = fork()) < 0)
		return (127);
	if (child == 0)
	{
		if ((change_exec(shell, path)))
			return (127);
	}
	else
	{
		waitpid(child, &shell->stop, 0);
		return (WEXITSTATUS(shell->stop));
	}
	return (0);
}

int		executable(t_shell *shell)
{
	char		*path;

	if ((path = getpath(shell)) == NULL)
		return (127);
	if (ft_lstsize(shell->pipeline) == 1)
		return (launch_exec(shell, path));
	else
		return (change_exec(shell, path));
}
