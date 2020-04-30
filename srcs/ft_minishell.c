/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 15:06:04 by mchardin          #+#    #+#             */
/*   Updated: 2020/04/30 16:32:51 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			main(int argc, char **argv, char **envp)
{
	t_shell		shell;
	struct stat stats;

	signal(SIGINT, sig_ctrl_c);
	signal(SIGQUIT, sig_ctrl_s);
	fstat(0, &stats);
	ft_mainargs(argc, argv, envp, &shell);
	while (line_loop(&shell, stats))
		;
	exit_end(&shell);
}
