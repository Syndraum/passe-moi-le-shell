/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 15:06:04 by mchardin          #+#    #+#             */
/*   Updated: 2020/01/09 18:01:38 by mchardin         ###   ########.fr       */
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
    char *buf;
    
    buf = 0;
    ft_putendl_fd(getcwd(buf, 0), 1);
    free(buf);
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
        ft_putstr_fd("minishell$ ", 1);
        if(get_next_line(0, &line) < 0)
            exit (0); //ERROR
        cursor = line;
        if (!(command = get_command(&cursor)))
            ft_putstr_fd("minishell : command not found\n", 2);
        else if (command == EXEC)
            stop = 0;
        else if (command == ECHO)
            stop = 0;
        else if (command == CD)
            stop = 0;
        else if (command == PWD)
            // stop = 0;
            command_pwd(&cursor);
        else if (command == EXPORT)
            stop = 0;
        else if (command == UNSET)
            stop = 0;
        else if (command == ENV)
            stop = 0;
        else if (command == EXIT)
            stop = 0;
    }
    exit (EXIT_SUCCESS);
}