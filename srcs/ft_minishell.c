/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roalvare <roalvare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 15:06:04 by mchardin          #+#    #+#             */
/*   Updated: 2020/02/19 14:29:49 by roalvare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_exit(t_shell *shell)
{
	ft_free_strs(shell->tab);
	free(shell->output);
	ft_free_strs(shell->env_keys);
	free(shell->pwd);
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
	pid_t	child;
	int		save_fd;
	t_list	*cmd;
	char	**tab;
	char	*path;
	char	**env;

	save_fd = 0;
	cmd = shell->pipeline;
	while (cmd != NULL)
	{
		tab = (char**)cmd->content;
		pipe(fd);
		if ((child = fork()) < 0)
			exit(1);
		if (child == 0)
		{
			dup2(save_fd, 0);
			if (cmd->next != NULL)
				dup2(fd[1], 1);
			close(fd[0]);
			shell->tab = cmd->content;
			if (get_command(tab[0]) == EXEC)
			{
				run_command(shell);
				exit(1);
			}
			else
			{
				if ((path = getpath(shell)) == NULL)
					exit(1);
				if (!(env = convert_env(shell)))
					exit(1);
				if (0 > execve(path, shell->tab, env))
				{
					ft_free_strs(env);
					exit(1);
				}
			}
		}
		else
		{
			wait(NULL);
			close(fd[1]);
			save_fd = fd[0];
			cmd = cmd->next;
		}
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
		ft_putstr_fd("\033[0;32mminishell$ \033[0m", 1);
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
				else
					loop_pipe(&shell);
				if (!shell.stop && shell.arg.sep != PIPE)
					ft_putstr_fd(shell.output, shell.fd);
			}
			if (keepreading == 0)
				exit (0); // FREE
			if (shell.arg.sep == END_LINE)
				stillcommand = 0;
			ft_lstclear(&shell.pipeline, free_tab_str);
		}
	}
	exit(EXIT_SUCCESS);
}
