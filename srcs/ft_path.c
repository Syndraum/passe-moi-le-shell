/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/20 18:43:55 by user42            #+#    #+#             */
/*   Updated: 2020/04/20 19:06:41 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
				&& (entry->d_type == DT_REG || entry->d_type == DT_LNK))
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

void	*free_retnull(void *to_free)
{
	free(to_free);
	return (NULL);
}

char	*getpath(t_shell *shell)
{
	char			**tab;
	int				i;
	char			*path;
	struct stat		sb;

	i = -1;
	if (!(tab = ft_split(
		get_item("PATH", shell->env_keys, shell->env_items), ':')))
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
	if (!(path = ft_strdup(shell->tab[0])))
		return (NULL);
	if (-1 == stat(path, &sb) || (!S_ISREG(sb.st_mode) && !S_ISLNK(sb.st_mode)))
		return (free_retnull(path));
	return (path);
}
