/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 15:06:04 by mchardin          #+#    #+#             */
/*   Updated: 2020/04/20 19:55:08 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		*get_separator(int sep)
{
	if (sep == TO_FILE)
		return (">");
	else if (sep == TO_END)
		return (">>");
	else if (sep == FROM_FILE)
		return ("<");
	else if (sep == SEMICOLON)
		return (";");
	else if (sep == PIPE)
		return ("|");
	return (NULL);
}

int			check_arg(t_shell *shell)
{
	char	**cursor;
	char	*sep;

	cursor = shell->cursor2;
	shell->arg.sep = 0;
	while (**cursor)
	{
		*cursor = skip_if(*cursor, ft_iswhitespace);
		shell->arg.str = get_argument(cursor, shell);
		shell->arg.sep = get_sep(cursor);
		if (!shell->arg.str && shell->arg.sep)
		{
			free(shell->arg.str);
			sep = get_separator(shell->arg.sep);
			print_error(shell, ERR_TOKEN, sep);
			return (0);
		}
		ft_freez((void**)&shell->arg.str);
		*cursor = skip_if(*cursor, ft_iswhitespace);
	}
	if (!shell->arg.str && is_redirection(shell->arg.sep))
	{
		print_error(shell, ERR_TOKEN, "'mewline'");
		return (0);
	}
	return (1);
}

int			main(int argc, char **argv, char **envp)
{
	t_shell		shell;
	struct stat stats;

	signal(SIGINT, sig_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
	fstat(0, &stats);
	ft_mainargs(argc, argv, envp, &shell);
	while (line_loop(&shell, stats))
		;
	exit_end(&shell);
}
