/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strs_add_end.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 12:46:41 by mchardin          #+#    #+#             */
/*   Updated: 2020/02/15 13:52:04 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char		**ft_strs_add_end(char **strs, char *add, int end)
{
	char		**new;

	if (!(new = malloc(sizeof(char*) * (end + 2))))
	{
		ft_free_strs(strs);
		return (0);
	}
	new[end + 1] = 0;
	new[end] = add;
	end--;
	while (end >= 0)
	{
		new[end] = strs[end];
		end--;
	}
	free(strs);
	return (new);
}
