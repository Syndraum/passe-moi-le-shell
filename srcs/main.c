/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roalvare <roalvare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 15:06:04 by mchardin          #+#    #+#             */
/*   Updated: 2020/01/10 16:51:16 by roalvare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    skip_char(char **cursor, char c)
{
    while(**cursor == c)
        (*cursor)++;
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
    skip_char(cursor, ' ');
    if (cmp_skip(cursor, ">"))
        return(TO_FILE);
    else if (cmp_skip(cursor, ">>"))
        return(TO_END);
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

int     get_command(char **cursor)
{
    skip_char(cursor, ' ');
    if (cmp_skip(cursor, "./"))
        return (EXEC);
    else if (cmp_skip(cursor, "echo"))
        return (ECHO);
    else if (cmp_skip(cursor, "cd"))
        return (CD);
    else if (cmp_skip(cursor, "pwd"))
        return (PWD);
    else if (cmp_skip(cursor, "export"))
        return (EXPORT);
    else if (cmp_skip(cursor, "unset"))
        return (UNSET);
    else if (cmp_skip(cursor, "env"))
        return (ENV);
    else if (cmp_skip(cursor, "exit"))
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
    }
    return (0);
}

int     main()
{
    int stop;
    char *line;
    char *cursor;
    t_command command;

    stop = 1;
    while (stop)
    {
        ft_putstr_fd("\033[0;32mminishell$ \033[0m", 1);
        if(get_next_line(0, &line) < 0)
            exit (0); //ERROR
        cursor = line;
        if (!(command = get_command(&cursor)))
            ft_putstr_fd("minishell : command not found\n", 2);
        else if (command == EXEC)
            stop = 1;
        else if (command == ECHO)
            ft_printf("%s", command_echo(&cursor));
        else if (command == CD)
            command_cd(&cursor);
        else if (command == PWD)
            command_pwd(&cursor);
        else if (command == EXPORT)
            stop = 1;
        else if (command == UNSET)
            stop = 1;
        else if (command == ENV)
            stop = 1;
        else if (command == EXIT)
            stop = 0;
    }
    exit (EXIT_SUCCESS);
}