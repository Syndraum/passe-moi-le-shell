/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roalvare <roalvare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 12:44:54 by roalvare          #+#    #+#             */
/*   Updated: 2020/01/10 17:52:59 by roalvare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*skip_if(char *str, int (*f)(char))
{
	while (f(*str))
		str++;
	return (str);
}

char	*command_echo(char **cursor)
{
	char	*arg;
	char	*tmp;
	int		carriage;

	carriage = 1;
	*cursor = skip_if(*cursor, is_whitespace);
	if (cmp_skip(cursor, "-n"))
	{
		carriage = 0;
		*cursor = skip_if(*cursor, is_whitespace);
	}
	tmp = get_argument(cursor);
	if (carriage)
	{
		arg = ft_calloc(ft_strlen(tmp) + 1, sizeof(char));
		ft_strlcpy(arg, tmp, ft_strlen(tmp) + 1);
		arg[ft_strlen(tmp)] = '\n';
		free(tmp);
	}
	else
		arg = tmp;
	return (arg);
}
