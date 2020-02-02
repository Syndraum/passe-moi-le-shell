/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roalvare <roalvare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 12:44:54 by roalvare          #+#    #+#             */
/*   Updated: 2020/02/02 15:20:14 by roalvare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*skip_if(char *str, int (*f)(char))
{
	while (*str != 0 && f(*str))
		str++;
	return (str);
}

int		command_echo(t_shell *shell)
{
	char	*ret;
	int		carriage;
	char	**cursor;

	carriage = 1;
	cursor = &shell->tab[1];
	ret = NULL;
	while (*cursor != 0 && cmp_skip(cursor, "-n"))
	{
		carriage = 0;
		cursor++;
	}
	while (*cursor != 0)
	{
		ret = ft_strjoin_gnl(ret, *cursor);
		cursor++;
		if (*cursor != 0)
			ret = ft_strjoin_gnl(ret, " ");
	}
	if (carriage)
		ret = ft_strjoin_gnl(ret, "\n");
	shell->output = ret;
	return (0);
}
