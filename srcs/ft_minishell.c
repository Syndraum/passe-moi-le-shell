/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 15:06:04 by mchardin          #+#    #+#             */
/*   Updated: 2020/03/02 20:06:10 by mchardin         ###   ########.fr       */
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

void		ft_open_line(char *file, t_shell *shell)
{
	if ((shell->fd_line = open(file, O_RDONLY)) < 0) //peut etre on peut le mettre dans fd_input en fait...
	{
		ft_printf("minishell: %s: %s\n", file, strerror(errno));
		exit (1);
	}
}

void		ft_first_init_struct(t_shell *shell)
{
	shell->env_keys = 0;
	shell->env_items = 0;
	shell->arg.str = 0;
	shell->output = 0;
	shell->pwd = 0;
	shell->oldpwd = 0;
	shell->pipeline = 0;
	shell->tab = 0;
	shell->line = 0;
	shell->cursor = 0;
	shell->fd_line = 0;
	shell->fd_input = 0;
	shell->fd_output = 1;
}

void		ft_mainargs(int argc, char **argv, char **envp, t_shell *shell)
{
	char	*buf;
	char	*tmp1;
	char	*tmp2;
	char	*tmp3;

	ft_first_init_struct(shell);
	if (argc > 1)
		ft_open_line(argv[1], shell);
	ft_env_lib(shell, envp);
	buf = 0;
	if (!(buf = getcwd(buf, 0)))
		exit_error(shell, 0);
	shell->pwd = buf;
	if (!(tmp1 = ft_strdup(buf)) || !(tmp2 = ft_strdup(argv[0])) || !(tmp3 = ft_shlvl(shell))
		|| !replace_or_add(&shell->env_keys, &shell->env_items,
		ft_strdup("PWD"), tmp1)
		|| !replace_or_add(&shell->env_keys, &shell->env_items,
		ft_strdup("_"), tmp2)
		|| !replace_or_add(&shell->env_keys, &shell->env_items,
		ft_strdup("SHLVL"), tmp3)
		|| !init_oldpwd(&shell->env_keys, &shell->env_items))
		exit_error(shell, 0);
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


// __attribute__((destructor)) void lul(void)
// {
// 	system("leaks minishell");
// }

int			main(int argc, char **argv, char **envp)
{
	t_shell		shell;
	int			keepreading;
	int			stillcommand;

	ft_mainargs(argc, argv, envp, &shell);
	while (1)
	{
		ft_putstr_fd("\033[0;32mminishell$ \033[0m", shell.fd_line);
		if ((keepreading = get_next_line(shell.fd_line, &shell.line)) < 0)
			exit_error(&shell, 0);
		shell.cursor = &shell.line;
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
			ft_lstclear(&shell.pipeline, free_cmd);
		}
	}
	exit_end(&shell);
}
