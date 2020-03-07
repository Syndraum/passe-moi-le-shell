/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executable.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/02 13:35:47 by mchardin          #+#    #+#             */
/*   Updated: 2020/03/07 19:39:07 by mchardin         ###   ########.fr       */
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
		if (ft_strncmp(keys[i], "_", 2))
			count += (items[i] != 0);
	return (count + 1);
}

char	**convert_env(t_shell *shell, char *path)
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
		if (ft_strncmp(shell->env_keys[i], "_", 2) && shell->env_items[i])
		{
			env[j] = ft_sprintf("%s=%s",
				shell->env_keys[i], shell->env_items[i]);
			j++;
		}
		i++;
	}
	env[j] = ft_sprintf("_=%s", path);
	env[j + 1] = 0;
	return (env);
}

char	*ft_lowercase(char *str)
{
	int		i;
	int		len;
	char	*lower;

	i = -1;
	len = strlen(str);
	if (!(lower = ft_calloc(sizeof(char), len + 1)))
		return (NULL);
	while (++i < len)
		lower[i] = ft_tolower(str[i]);
	return (lower);
}

int		ft_strncmp_case(char *s1, char *s2, size_t n)
{
	char	*lower_s1;
	char	*lower_s2;
	int		cmp;

	if (!(lower_s1 = ft_lowercase(s1)))
		return (-1);
	if (!(lower_s2 = ft_lowercase(s2)))
	{
		ft_freez((void **)&lower_s1);
		return (-1);
	}
	cmp = ft_strncmp(lower_s1, lower_s2, n);
	ft_freez((void **)&lower_s1);
	ft_freez((void **)&lower_s2);
	return (cmp);
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
			if (ft_strncmp_case(entry->d_name, filename, len) == 0
				&& entry->d_type == DT_REG)
			{
				closedir(dir);
				if (!(find = path_dir[ft_strlen(path_dir) - 1] == '/' ?
				ft_sprintf("%s%s", path_dir, filename)
				: ft_sprintf("%s/%s", path_dir, filename)))
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
			ft_free_strs(&tab);
			return (path);
		}
	}
	ft_free_strs(&tab);
	if (!(path = ft_sprintf("%s%c%s", shell->pwd, '/', shell->tab[0])))
		return (NULL);
	if (-1 == stat(path, &sb))
	{
		free(path);
		return (NULL);
	}
	return (path);
}

int		change_exec(t_shell *shell, char *path)
{
	char		**env;

	if (!(env = convert_env(shell, path)))
		return (127);
	if (0 > execve(path, shell->tab, env))
		return (127);
	return (0);
}

int		launch_exec(t_shell *shell, char *path)
{
	pid_t		child;
	int			fd[2];
	int			fd_input[2];

	if ((child = fork()) < 0)
		return (127);
	if (child == 0)
	{
		if (shell->fd_output != 1)
		{
			pipe(fd);
			dup2(shell->fd_output, 1);
			close(fd[0]);
		}
		if (shell->fd_input != 0)
		{
			pipe(fd_input);
			dup2(shell->fd_input, 0);
			close(fd_input[0]);
		}
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
	int			code;

	if ((path = getpath(shell)) == NULL)
		return (127);
	if (ft_lstsize(shell->pipeline) == 1)
		code = launch_exec(shell, path);
	else
		code = change_exec(shell, path);
	ft_freez((void**)&path);
	return (code);
}
