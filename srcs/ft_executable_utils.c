/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executable_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 12:48:35 by chucky            #+#    #+#             */
/*   Updated: 2020/04/20 18:44:22 by user42           ###   ########.fr       */
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
