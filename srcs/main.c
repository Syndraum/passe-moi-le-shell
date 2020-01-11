/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 15:06:04 by mchardin          #+#    #+#             */
/*   Updated: 2020/01/11 20:03:08 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    skip_char(char **cursor, char c)
{
    while(**cursor == c)
        (*cursor)++;
}    

int		ft_is_var_def(char *var)
{
	int i;
	
	i = 0;
	if(!(ft_isalpha(var[i])))
		return (0);
	while(ft_isalpha(var[i]))
		i++;
		if (var[i] != '=')
			return(0);
	return (1);
}

int	ft_cmp(char *str, char *cursor)
{
	if (ft_strncmp(str, cursor, ft_strlen(cursor)))
        return (0);
	return (1);
}

int     cmp_skip(char **cursor, char *str)
{
    if (ft_strncmp(str, *cursor, ft_strlen(str)))
        return (0);
    *(cursor) += ft_strlen(str);
    return (1);
}

int     get_arg(char **cursor)
{
    *cursor = skip_if(*cursor, is_whitespace);
    if (cmp_skip(cursor, ">>"))
        return(TO_END);
    else if (cmp_skip(cursor, ">"))
        return(TO_FILE);
    else if (cmp_skip(cursor, "<"))
        return(FROM_FILE);
    else if (cmp_skip(cursor, ";"))
        return(SEMICOLON);
    else if (cmp_skip(cursor, "|"))
        return(PIPE);
    else if (ft_isprint(**cursor))
        return (ARG);
    else
        return (0);
}

int     get_command(char *command)
{
    if (ft_cmp(command, "./"))
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

int     command_pwd(char **cursor)
{
    char    *buf;
    int     separator;

    separator = get_arg(cursor);
    buf = 0;
    ft_putendl_fd(getcwd(buf, 0), 1);
    free(buf);
    return (0);
}

int     command_cd(char **cursor)
{
    int     separator;
    separator = get_arg(cursor);
    if (chdir(*cursor) < 0)
    {
        ft_dprintf(2, "%s\n", strerror(errno));
		return (0);
    }
    return (1);
}

int64_t	command_export(t_shell *shell)
{
	int		i;

	i = 0;
	while(shell->tab[i])
	{
		if(!(ft_is_var_def(shell->tab[i])))
			return (0);
		i++;
	}
	i = 0;
	while(shell->tab[i])
	{
		if(!(shell->environ = ft_strs_plus_one(shell->environ, shell->tab[i])))
		{
			ft_dprintf(2, "%s\n", strerror(errno));
			return (0);
		}
		i++;
	}
	return (1);
}

void	command_env(t_shell *shell)
{
	int		i;

	i = 0;
	while (shell->environ[i])
	{
		ft_putendl_fd(shell->environ[i], shell->fd);
		i++;
	}
}

int		ft_mainargs(int argc, char **argv, char **envp, t_shell *shell)
{
	(void)argc;
	(void)argv;
	if(!(shell->environ = ft_strs_cpy(envp)))
	{
		ft_dprintf(2, "%s\n", strerror(errno));
		return (0);
	}
	return (1);
}

int     main(int argc, char **argv, char **envp)
{
    int stop;
    char *line;
	t_shell shell;

    stop = 1;
	if(!(ft_mainargs(argc, argv, envp, &shell)))
		return (0);
    while (stop)
    {
        ft_putstr_fd("\033[0;32mminishell$ \033[0m", 1);
        if(get_next_line(0, &line) < 0)
            exit (0); //ERROR
        shell.cursor = &line;
		analyse_args(&shell);
        if (!(shell.command = get_command(shell.tab[0])))
            ft_putstr_fd("minishell : command not found\n", 2);
        else if (shell.command == EXEC)
            stop = 1;
        else if (shell.command == ECHO)
            shell.output = command_echo(&shell);
        else if (shell.command == CD)
            command_cd(shell.cursor);
        else if (shell.command == PWD)
            command_pwd(shell.cursor);
        else if (shell.command == EXPORT)
            command_export(&shell);
        else if (shell.command == UNSET)
            stop = 1;
        else if (shell.command == ENV)
            command_env(&shell);
        else if (shell.command == EXIT)
            stop = 0;
		if (shell.arg.sep != PIPE)
			ft_putstr_fd(shell.output, shell.fd);
    }
    exit (EXIT_SUCCESS);
}
