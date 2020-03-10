/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gnl_minishell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 12:36:58 by mchardin          #+#    #+#             */
/*   Updated: 2020/03/09 14:16:35 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				gnl_minishell(int fd, char **line)
{
	static t_buf	tab = {.buf[0][0] = -1, .r = 0};
	int				i;
	char			*str;

	str = 0;
	if (tab.buf[0][0] == -1 && (!line || B <= 0
		|| !(ft_first_read(&tab, fd, &i))))
		return (-1);
	else
		i = B;
	while (ft_check(tab.buf[0]) == -1 && i == B && !tab.r)
	{
		if (!(ft_str_gnl(&str, &tab, fd, &i)))
			return (-1);
		else if (signal(SIGINT, SIG_IGN))
			return (2);
	}
	if (!(ft_copy(line, &tab, &str)))
		return (-1);
	if (tab.r == 1 && tab.buf[0][0] == 0)
	{
		tab.buf[0][0] = -1;
		tab.r = 0;
		return (0);
	}
	else if (signal(SIGINT, SIG_IGN))
		return (2);
	return (1);
}
