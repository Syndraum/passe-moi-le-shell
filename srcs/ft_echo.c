/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roalvare <roalvare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 12:44:54 by roalvare          #+#    #+#             */
/*   Updated: 2020/01/11 13:54:16 by roalvare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*skip_if(char *str, int (*f)(char))
{
	while (f(*str))
		str++;
	return (str);
}

char	*command_echo(t_shell *shell)
{
	char	*ret;
	int		carriage;
	int		len;
	char	**cursor;
	char	**begin;

	carriage = 1;
	cursor = &shell->tab[1];
	while (cmp_skip(cursor, "-n"))
	{
		carriage = 0;
		cursor++;
	}
	begin = cursor;
	len = 0;
	while (*cursor != 0)
	{
		len += ft_strlen(*cursor) + 1;
		cursor++;
	}
	if (!(ret = ft_calloc(len + carriage, sizeof(char))))
		return (NULL);
	while (*begin != 0)
	{
		ft_strlcat(ret, (*begin), ft_strlen(ret) + ft_strlen(*begin) + 1);
		begin++;
		if (*begin != 0)
			ft_strlcat(ret, " ", ft_strlen(ret) + 2);
	}
	if (carriage)
		ft_strlcat(ret, "\n", ft_strlen(ret) + 2);
	return (ret);
}
