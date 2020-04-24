/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 15:06:04 by mchardin          #+#    #+#             */
/*   Updated: 2020/04/22 22:37:01 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			main(int argc, char **argv, char **envp)
{
	t_shell		shell;
	struct stat stats;

	signal(SIGINT, sig_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
	fstat(0, &stats);
	ft_mainargs(argc, argv, envp, &shell);
	while (line_loop(&shell, stats))
		;
	exit_end(&shell);
}
