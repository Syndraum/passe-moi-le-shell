/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 15:06:04 by mchardin          #+#    #+#             */
/*   Updated: 2020/02/02 11:26:20 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			get_command(char *command)
{
	if (ft_cmp(command, "./")) // MORE GENERAL
		return (EXEC);
	else if (ft_cmp(command, "echo"))
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
		return (0);
}

char		*ft_shlvl(char **environ)
{
	int		i;
	int		j;
	int		ret;

	i = 0;
	while (environ[i])
	{
		j = 6;
		if (is_var_to_unset("SHLVL", environ[i]))
		{
			while (environ[i][j])
			{
				if (!(ft_isdigit(environ[i][j]) || environ[i][j] == '+'
					|| environ[i][j] == '-'))
					return (ft_strdup("1"));
				j++;
			}
			ret = ft_atoi(&environ[i][6]);
			return (ret < 0 ? ft_strdup("0") : ft_itoa(ret + 1));
		}
		i++;
	}
	return (ft_strdup("1"));
}

int			ft_mainargs(int argc, char **argv, char **envp, t_shell *shell)
{
	char	*buf;

	(void)argc;
	(void)argv;
	if (!(shell->environ = ft_strs_cpy(envp)))
	{
		ft_dprintf(2, "%s\n", strerror(errno));
		return (0);
	}
	buf = 0;
	if (!(buf = getcwd(buf, 0)))
		return (0); //ERROR MESSAGE ??
	shell->pwd = buf;
	if (!(buf = ft_strjoin("PWD=", shell->pwd)) ||
		!replace_or_add(shell->environ, buf)
		|| !(buf = ft_strjoin("_=", argv[0])) ||
		!replace_or_add(shell->environ, buf)
		|| !(buf = ft_strjoin("SHLVL=", ft_shlvl(shell->environ)))
		|| !replace_or_add(shell->environ, buf))
		return (0);
	return (1);
}

int			run_command(t_shell *shell)
{
	int		ret;

	ret = 1;
	if (!(shell->command = get_command(shell->tab[0])))
		ft_putstr_fd("minishell : command not found\n", 2);
	else if (shell->command == EXEC)
		return (1);
	else if (shell->command == ECHO)
		shell->output = command_echo(shell);
	else if (shell->command == CD)
		ret = command_cd(shell);
	else if (shell->command == PWD)
		command_pwd(shell);
	else if (shell->command == EXPORT)
		ret = command_export(shell);
	else if (shell->command == UNSET)
		ret = command_unset(shell);
	else if (shell->command == ENV)
		command_env(shell);
	else if (shell->command == EXIT)
		return (0);
	return (ret);
}

int			main(int argc, char **argv, char **envp)
{
	int			stop;
	char		*line;
	t_shell		shell;

	stop = 1;
	if (!(ft_mainargs(argc, argv, envp, &shell)))
		return (0);
	while (stop)
	{
		ft_putstr_fd("\033[0;32mminishell$ \033[0m", 1);
		if (get_next_line(0, &line) < 0)
			exit (0); //ERROR
		shell.cursor = &line;
		analyse_args(&shell);
		if (!(last_arg_env(shell.environ, shell.tab)))
			return (0); // ERROR MALLOC
		stop = run_command(&shell);
		if (shell.arg.sep != PIPE)
			ft_putstr_fd(shell.output, shell.fd);
	}
	exit(EXIT_SUCCESS);
}
