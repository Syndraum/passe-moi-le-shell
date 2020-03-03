/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 15:06:04 by mchardin          #+#    #+#             */
/*   Updated: 2020/03/03 19:38:40 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			get_command(char *command)
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

int			run_command(t_shell *shell)
{
	int ret;

	shell->command = get_command(shell->tab[0]);
	if (shell->command == EXEC)
	{
		ret = executable(shell);
		if (ret == 127)
			ft_dprintf(2, "minishell : %s: %s\n", shell->tab[0], ERR_CMD_NF);
		return (ret);
	}
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

void		loop_pipe(t_shell *shell)
{
	int		fd[2];
	int		fd_input[2];
	pid_t	child;
	int		save_fd;
	t_list	*elmt;
	t_cmd 	*cmd;

	save_fd = 0;
	elmt = shell->pipeline;
	while (elmt != NULL)
	{
		cmd = (t_cmd*)elmt->content;
		pipe(fd);
		if ((child = fork()) < 0)
			exit(1);
		if (child == 0)
		{
			shell->tab = cmd->arg;
			shell->fd_input = cmd->fd_input;
			shell->fd_output = cmd->fd_output;
			if (shell->fd_input != 0)
			{
				pipe(fd_input);
				dup2(shell->fd_input, 0);
				close(fd_input[0]);
			}
			dup2(save_fd, 0);
			if (shell->fd_output != 1)
				dup2(shell->fd_output, 1);
			else if (elmt->next != NULL)
				dup2(fd[1], 1);
			close(fd[0]);
			shell->stop = run_command(shell);
			ft_putstr_fd(shell->output, 1);
			exit(shell->stop);
		}
		else
		{
			waitpid(child, &shell->stop, 0);
			shell->stop = WEXITSTATUS(shell->stop);
			close(fd[1]);
			save_fd = fd[0];
			elmt = elmt->next;
		}
	}
}

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

__attribute__((destructor)) void lul(void)
{
	system("leaks minishell");
}

int			main(int argc, char **argv, char **envp)
{
	t_shell		shell;
	int			keepreading;
	int			stillcommand;

	ft_mainargs(argc, argv, envp, &shell);
	while (1)
	{
		ft_putstr_fd("\033[0;32mminishell$ \033[0m", shell.fd_line);
		if ((keepreading = get_next_line(shell.fd_line, shell.line)) < 0)
			exit_error(&shell, 0);
		shell.cursor[0] = shell.line[0];
		stillcommand = 1;
		while (stillcommand)
		{
			shell.pipeline = NULL;
			if (analyse_args(&shell))
			{
				if (!(last_arg_env(&shell.env_keys, &shell.env_items, shell.tab)))
					exit_error(&shell, shell.tab[0]);
				if (ft_lstsize(shell.pipeline) == 1)
					shell.stop = run_command(&shell);
				else if (ft_lstsize(shell.pipeline) > 1)
					loop_pipe(&shell);
				if (!shell.stop && ft_lstsize(shell.pipeline) == 1)
					ft_putstr_fd(shell.output, shell.fd_output);
			}
			if (keepreading == 0)
				exit_end(&shell);
			if (shell.arg.sep == END_LINE)
				stillcommand = 0;
			free_line(&shell);
		}
	}
	exit_end(&shell);
}
