/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 16:47:33 by mchardin          #+#    #+#             */
/*   Updated: 2020/03/04 16:47:53 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tab_str(char **tab)
{
	int i;

	if (tab == NULL)
		ft_printf("NULL\n");
	else
	{
		i = -1;
		while (tab[++i])
			ft_printf("[%d]:%s\n", i, tab[i]);
	}
}

void	print_list_tab(t_list *list)
{
	t_list	*cursor;

	cursor = list;
	while (cursor != NULL)
	{
		print_tab_str((char**)cursor->content);
		cursor = cursor->next;
	}
}
