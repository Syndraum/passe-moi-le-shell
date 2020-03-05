/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_find_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roalvare <roalvare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 18:04:35 by roalvare          #+#    #+#             */
/*   Updated: 2020/03/05 18:08:57 by roalvare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		get_tabidx(char *key, char **keys)
{
	int i;
	int len;

	i = -1;
	len = ft_strlen(key);
	while (keys[++i] != 0)
	{
		if (!ft_strncmp(key, keys[i], len + 1))
			return (i);
	}
	return (-1);
}

char	*get_item(char *key, char **keys, char **items)
{
	int		index;

	if ((index = get_tabidx(key, keys)) == -1)
		return (0);
	return (items[index]);
}
