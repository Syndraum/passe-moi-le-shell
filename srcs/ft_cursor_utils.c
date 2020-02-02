/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cursor_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/02 10:28:47 by mchardin          #+#    #+#             */
/*   Updated: 2020/02/02 11:17:52 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		skip_char(char **cursor, char c)
{
	while (**cursor == c)
		(*cursor)++;
}

int			ft_cmp(char *str, char *cursor)
{
	if (ft_strncmp(str, cursor, ft_strlen(cursor)))
		return (0);
	return (1);
}

int			cmp_skip(char **cursor, char *str)
{
	if (ft_strncmp(str, *cursor, ft_strlen(str)))
		return (0);
	*(cursor) += ft_strlen(str);
	return (1);
}

int    		 get_arg(char **cursor)
{
    *cursor = skip_if(*cursor, ft_iswhitespace);
    if (cmp_skip(cursor, ">>"))
        return(TO_END);
    else if (cmp_skip(cursor, ">"))
        return(TO_FILE);
    else if (cmp_skip(cursor, "<"))
        return(FROM_FILE);
    else if (cmp_skip(cursor, ";"))
        return(SEMICOLON);
    else if (cmp_skip(cursor, "|"))
        return(PIPE);
    else if (ft_isprint(**cursor))
        return (ARG);
    else
        return (0);
}