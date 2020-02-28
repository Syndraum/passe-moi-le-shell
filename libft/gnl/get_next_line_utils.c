/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 21:28:24 by mchardin          #+#    #+#             */
/*   Updated: 2020/02/28 15:43:05 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"

char	*ft_strjoin_gnl(char *s1, char *s2)
{
	unsigned int	i;
	unsigned int	j;
	char			*str;
	char			*temp_s1;

	i = 0;
	j = 0;
	temp_s1 = s1;
	while (s1 && s1[i])
		i++;
	while (s2 && s2[j])
		j++;
	if (!(str = malloc(sizeof(char*) * (i + j + 1))))
	{
		ft_freez(s1);
		return (0);
	}
	while (s1 && *temp_s1)
		*str++ = *temp_s1++;
	while (s2 && *s2)
		*str++ = *s2++;
	*str = 0;
	ft_freez(s1);
	return (str - i - j);
}
