/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roalvare <roalvare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 15:06:04 by mchardin          #+#    #+#             */
/*   Updated: 2020/02/14 18:49:45 by roalvare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_exit(t_shell *shell)
{
	ft_free_strs(shell->tab);
	free(shell->output);
	ft_free_strs(shell->environ);
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
	// (void)argv;
	ft_env_lib(shell, envp);
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
		!replace_or_add(&shell->env_keys, &shell->env_items, buf)
		|| !(buf = ft_strjoin("_=", argv[0])) ||
		!replace_or_add(&shell->env_keys, &shell->env_items, buf)
		|| !(buf = ft_strjoin("SHLVL=", ft_shlvl(shell->environ)))
		|| !replace_or_add(&shell->env_keys, &shell->env_items, buf)
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

int			main(int argc, char **argv, char **envp)
{
	char		*line;
	t_shell		shell;
	int			keepreading;
	int			keepcommand;

	if (!(ft_mainargs(argc, argv, envp, &shell)))
		return (0);
	while (1)
	{
		ft_putstr_fd("\033[0;32mminishell$ \033[0m", 1);
		if ((keepreading = get_next_line(0, &line)) < 0)
			exit (0); //ERROR
		shell.cursor = &line;
		keepcommand = 1;
		while (keepcommand)
		{
			// ft_printf("line  %s\n", *(shell.cursor));
			if (analyse_args(&shell))
			{
				if (!(last_arg_env(&shell.env_keys, &shell.env_items, shell.tab)))
					return (0); // ERROR MALLOC
				shell.stop = run_command(&shell);
				if (!shell.stop && shell.arg.sep != PIPE)
					ft_putstr_fd(shell.output, shell.fd);
			}
			if (keepreading == 0)
				exit (0); // FREE
			if (shell.arg.sep == END_LINE)
				keepcommand = 0;
		}
	}
	exit(EXIT_SUCCESS);
}
