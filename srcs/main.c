/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 15:06:04 by mchardin          #+#    #+#             */
/*   Updated: 2020/01/12 12:40:43 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void   		 skip_char(char **cursor, char c)
{
    while(**cursor == c)
        (*cursor)++;
}    

int			ft_cmp(char *str, char *cursor)
{
	if (ft_strncmp(str, cursor, ft_strlen(cursor)))
        return (0);
	return (1);
}

int    		 cmp_skip(char **cursor, char *str)
{
    if (ft_strncmp(str, *cursor, ft_strlen(str)))
        return (0);
    *(cursor) += ft_strlen(str);
    return (1);
}

int    		 get_arg(char **cursor)
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

int  		   get_command(char *command)
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

int			ft_mainargs(int argc, char **argv, char **envp, t_shell *shell)
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

int   		  main(int argc, char **argv, char **envp)
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
            command_cd(&shell);
        else if (shell.command == PWD)
            command_pwd(&shell);
        else if (shell.command == EXPORT)
            command_export(&shell);
        else if (shell.command == UNSET)
            command_unset(&shell);
        else if (shell.command == ENV)
            command_env(&shell);
        else if (shell.command == EXIT)
            stop = 0;
		if (shell.arg.sep != PIPE)
			ft_putstr_fd(shell.output, shell.fd);
    }
    exit (EXIT_SUCCESS);
}
