/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/20 19:25:20 by mchardin          #+#    #+#             */
/*   Updated: 2020/04/22 21:51:26 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		get_command(char *command)
{
	if (ft_cmp(command, "echo"))
		return (ECHO);
	else if (ft_cmp(command, "cd"))
		return (CD);
	else if (ft_cmp(command, "pwd"))
		return (PWD);
	else if (ft_cmp(command, "export"))
		return (EXPORT);
	else if (ft_cmp(command, "unset"))
		return (UNSET);
	else if (ft_cmp(command, "env"))
		return (ENV);
	else if (ft_cmp(command, "exit"))
		return (EXIT);
	else
		return (EXEC);
}

int		run_exec(t_shell *shell)
{
	int ret;

	ret = executable(shell);
	if (ret == 127)
		print_error(shell, shell->tab[0], ERR_CMD_NF);
	else if (ret == 126)
		print_error(shell, shell->tab[0], ERR_FILE_DIR);
	return (ret);
}

int		run_command(t_shell *shell)
{
	if (shell->fd_input == -1 || shell->fd_output == -1)
		return (1);
	shell->command = get_command(shell->tab[0]);
	if (shell->command == EXEC)
		return (run_exec(shell));
	else if (shell->command == ECHO)
		return (command_echo(shell));
	else if (shell->command == CD)
		return (command_cd(shell));
	else if (shell->command == PWD)
		return (command_pwd(shell));
	else if (shell->command == EXPORT)
		return (command_export(shell));
	else if (shell->command == UNSET)
		return (command_unset(shell));
	else if (shell->command == ENV)
		return (command_env(shell));
	else if (shell->command == EXIT)
		exit_end(shell);
	return (1);
}
