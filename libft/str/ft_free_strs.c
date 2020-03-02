/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_strs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 12:39:44 by mchardin          #+#    #+#             */
/*   Updated: 2020/03/02 17:56:13 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

void	ft_free_strs(char ***grid)
{
	int	i;

	i = 0;
	if (!grid[0])
		return ;
	while (grid[0][i])
	{
		ft_freez((void **)&grid[0][i]);
		i++;
	}
	ft_freez((void **)grid[0]);
}
