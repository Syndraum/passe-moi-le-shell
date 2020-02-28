/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 15:06:04 by mchardin          #+#    #+#             */
/*   Updated: 2020/02/28 21:43:43 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_exit(t_shell *shell)
{
	ft_free_strs(shell->tab);
	ft_freez(shell->output);
	ft_free_strs(shell->env_keys);
	ft_freez(shell->pwd);
}

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

char		*ft_shlvl(t_shell *shell)
{
	int		idx;
	int		ret;
	int		j;

	j = 1;
	idx = get_tabidx("SHLVL", shell->env_keys);
	if (idx >= 0)
	{
		if (!(ft_isdigit(shell->env_items[idx][0])
			|| shell->env_items[idx][0] == '+'
			|| shell->env_items[idx][0] == '-'))
			return (ft_strdup("1"));
		while (shell->env_items[idx][j])
		{
			if (!(ft_isdigit(shell->env_items[idx][j])))
				return (ft_strdup("1"));
			j++;
		}
		ret = ft_atoi(shell->env_items[idx]);
		return (ret < 0 ? ft_strdup("0") : ft_itoa(ret + 1));
	}
	return (ft_strdup("1"));
}

int			ft_mainargs(int argc, char **argv, char **envp, t_shell *shell)
{
	char	*buf;

	(void)argc;
	ft_env_lib(shell, envp);
	buf = 0;
	if (!(buf = getcwd(buf, 0)))
		return (0); //ERROR MESSAGE ??
	shell->pwd = buf;
	if (!replace_or_add(&shell->env_keys, &shell->env_items, ft_strdup("PWD"), ft_strdup(buf))
		|| !replace_or_add(&shell->env_keys, &shell->env_items, ft_strdup("_"), ft_strdup(argv[0]))
		|| !replace_or_add(&shell->env_keys, &shell->env_items, ft_strdup("SHLVL"), ft_shlvl(shell))
		|| !init_oldpwd(&shell->env_keys, &shell->env_items))
		return (0);
	return (1);
}

int			run_command(t_shell *shell)
{
	int ret;

	shell->command = get_command(shell->tab[0]);
	if (shell->command == EXEC)
	{
		ret = executable(shell);
		if (ret == 127)
			ft_putstr_fd("minishell : command not found\n", 2);
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
		exit(0);
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

int			main(int argc, char **argv, char **envp)
{
	char		*line;
	t_shell		shell;
	int			keepreading;
	int			stillcommand;

	if (!(ft_mainargs(argc, argv, envp, &shell)))
		return (0);
	while (1)
	{
		ft_putstr_fd("\033[0;32mminishell$ \033[0m", 0);
		if ((keepreading = get_next_line(0, &line)) < 0)
			exit (0); //ERROR
		shell.cursor = &line;
		stillcommand = 1;
		while (stillcommand)
		{
			shell.pipeline = NULL;
			if (analyse_args(&shell))
			{
				if (!(last_arg_env(&shell.env_keys, &shell.env_items, shell.tab)))
					return (0); // ERROR MALLOC
				if (ft_lstsize(shell.pipeline) == 1)
					shell.stop = run_command(&shell);
				else if (ft_lstsize(shell.pipeline) > 1)
					loop_pipe(&shell);
				if (!shell.stop && ft_lstsize(shell.pipeline) == 1)
					ft_putstr_fd(shell.output, shell.fd_output);
			}
			if (keepreading == 0)
				exit (0); // FREE
			if (shell.arg.sep == END_LINE)
				stillcommand = 0;
			ft_lstclear(&shell.pipeline, free_cmd);
		}
	}
	exit(EXIT_SUCCESS);
}
