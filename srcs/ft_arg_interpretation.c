/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arg_interpretation.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roalvare <roalvare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/10 19:48:21 by mchardin          #+#    #+#             */
/*   Updated: 2020/03/09 12:34:09 by roalvare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_redirection(t_shell *shell, t_separator prev)
{
	int ret;

	ret = 1;
	if (shell->fd_input > 2 && prev == FROM_FILE)
		close(shell->fd_input);
	else if (shell->fd_output > 2 && (prev == TO_FILE || prev == TO_END))
		close(shell->fd_output);
	if (prev == TO_FILE && (shell->fd_output = open(shell->arg.str,
		O_CREAT | O_WRONLY | O_TRUNC, FILE_RIGHTS)) < 0)
		ft_printf("minishell: %s: %s\n", shell->arg.str, strerror(errno));
	else if (prev == TO_END && (shell->fd_output = open(shell->arg.str,
		O_CREAT | O_WRONLY | O_APPEND, FILE_RIGHTS)) < 0)
		ft_printf("minishell: %s: %s\n", shell->arg.str, strerror(errno));
	else if (prev == FROM_FILE
		&& (shell->fd_input = open(shell->arg.str, O_RDONLY)) < 0)
		ft_printf("minishell: %s: %s\n", shell->arg.str, strerror(errno));
	ft_freez((void **)&shell->arg.str);
	if (shell->fd_input == -1 || shell->fd_output == -1)
		ret = 0;
	return (ret);
}

int		arg_loop(t_shell *shell)
{
	t_separator		prev;

	prev = shell->arg.sep;
	if (!(set_arg(shell)))
	{
		ft_free_strs(&shell->tab);
		return (0);
	}
	if (prev == ARG &&
	!(shell->tab = ft_strs_plus_one(shell->tab, shell->arg.str)))
		exit_error(shell, 0);
	else if (is_redirection(prev) && !(ft_redirection(shell, prev)))
		return (0);
	return (1);
}

void	skip_redirection(t_shell *shell)
{
	while (!is_end_of_command(shell->arg.sep))
		free(set_arg(shell));
}

int		set_pipeline(t_shell *shell)
{
	t_list		*elmt;
	t_cmd		*command;

	init_struct(shell);
	while (1)
	{
		if (!(init_tab(shell)))
			return (0);
		while (!is_end_of_command(shell->arg.sep))
			if (!arg_loop(shell))
				skip_redirection(shell);
		if (!(command = malloc(sizeof(t_cmd))))
			exit_error(shell, 0);
		command->arg = shell->tab;
		command->fd_input = shell->fd_input;
		command->fd_output = shell->fd_output;
		if (!(elmt = ft_lstnew(command)))
			exit_error(shell, 0);
		ft_lstadd_back(&shell->pipeline, elmt);
		if (shell->arg.sep != PIPE)
			break ;
	}
	return (1);
}

int		analyse_args(t_shell *shell)
{
	t_list	*elmt;
	int		ret;

	elmt = NULL;
	while ((ret = set_pipeline(shell)) && shell->arg.sep != END_LINE)
	{
		if (!(elmt = ft_lstnew(shell->pipeline)))
			exit_error(shell, 0);
		ft_lstadd_back(&shell->lists, elmt);
	}
	if (!ret && shell->arg.sep == PIPE)
		ft_dprintf(2, "minishell: syntax error near unexpected token `|'\n");
	if (!ret && shell->arg.sep == SEMICOLON)
		ft_dprintf(2, "minishell: syntax error near unexpected token `;'\n");
	return (ret);
}
