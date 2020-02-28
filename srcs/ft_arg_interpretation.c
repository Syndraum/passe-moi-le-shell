/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arg_interpretation.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roalvare <roalvare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 19:48:21 by mchardin          #+#    #+#             */
/*   Updated: 2020/02/28 19:42:15 by roalvare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_end_of_command(t_separator sep)
{
	if (sep == END_LINE || sep == SEMICOLON || sep == PIPE)
		return (1);
	return (0);
}

int		is_redirection(t_separator sep)
{
	if (sep == TO_END || sep == TO_FILE || sep == FROM_FILE)
		return (1);
	return (0);
}

void	init_struct(t_shell *shell)
{
	shell->output = 0;
	shell->fd_input = 0;
	shell->fd_output = 1;
}

int		init_tab(t_shell *shell)
{
	shell->fd_input = 0;
	shell->fd_output = 1;
	if (!(set_arg(shell)))
	{
		shell->tab = 0;
		return (0); // EXIT ? RETURN ? 
	}
	if (!(shell->tab = malloc(sizeof(char*) * 2)))
	{
		free(shell->arg.str);
		exit(1);
	}
	shell->tab[0] = shell->arg.str;
	shell->tab[1] = NULL;
	return (1);
}

void	free_cmd(void *content)
{
	t_cmd * cmd;

	cmd = (t_cmd*)content;
	if (cmd->fd_output != 1)
		close(cmd->fd_output);
	ft_free_strs(cmd->arg);
}

int		ft_redirection(t_shell *shell, t_separator prev)
{
	if (shell->fd_input != 0 && prev == FROM_FILE)
		close(shell->fd_input);
	if (shell->fd_output != 1 && (prev == TO_FILE || prev == TO_END))
		close(shell->fd_output);
	if (prev == TO_FILE && (shell->fd_output = open(shell->arg.str,
		O_CREAT | O_WRONLY | O_TRUNC, FILE_RIGHTS)) < 0)
	{
		ft_printf("minishell: %s: %s\n", shell->arg.str, strerror(errno));
		return (0); //NOT EXIT
	}
	else if (prev == TO_END && (shell->fd_output = open(shell->arg.str,
		O_CREAT | O_WRONLY | O_APPEND, FILE_RIGHTS)) < 0)
	{
		ft_printf("minishell: %s: %s\n", shell->arg.str, strerror(errno));
		return (0); //NOT EXIT
	}
	else if (prev == FROM_FILE && (shell->fd_input = open(shell->arg.str, O_RDONLY)) < 0)
	{
		ft_printf("minishell: %s: %s\n", shell->arg.str, strerror(errno));
		return (0); //NOT EXIT
	}
	// ft_printf("output : %d\tinput : %d\n", shell->fd_output, shell->fd_input);
	free(shell->arg.str);
	return (1);
}

int		analyse_args(t_shell *shell)
{
	t_separator prev;
	t_list		*elmt;
	t_cmd	*command;
	int			pipeline;

	pipeline = 1;
	init_struct(shell);
	while (pipeline)
	{
		if (!(init_tab(shell)))
			return (0);
		while (!is_end_of_command(shell->arg.sep))
		{
			prev = shell->arg.sep;
			if (!(set_arg(shell)))
			{
				ft_free_strs(shell->tab);
				return (0);
			}
			if (prev == ARG &&
			!(shell->tab = ft_strs_plus_one(shell->tab, shell->arg.str)))
				return (0);
			else if (is_redirection(prev) && !(ft_redirection(shell, prev)))
				return (0);
		}
		if (!(command = malloc(sizeof(t_cmd))))
			return (0);
		command->arg = shell->tab;
		command->fd_input = shell->fd_input;
		command->fd_output = shell->fd_output;
		if (!(elmt = ft_lstnew(command)))
			return (0);
		ft_lstadd_back(&shell->pipeline, elmt);
		if (shell->arg.sep != PIPE)
			pipeline = 0;
	}
	return (1);
}
