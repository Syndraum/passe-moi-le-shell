/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executable.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/02 13:35:47 by mchardin          #+#    #+#             */
/*   Updated: 2020/04/30 16:45:17 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		change_exec(t_shell *shell, char *path)
{
	char		**env;

	if (!(env = convert_env(shell, path)))
		return (1);
	if (0 > execve(path, shell->tab, env))
		return (1);
	return (0);
}

void	lauch_child(t_shell *shell, char *path)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (shell->fd_output > 2)
		dup2(shell->fd_output, 1);
	if (shell->fd_input > 2)
		dup2(shell->fd_input, 0);
	if ((change_exec(shell, path)))
		exit(127);
}

int		error_child(char *str, int ret)
{
	ft_putstr_fd(str, 0);
	return (ret);
}

int		launch_exec(t_shell *shell, char *path)
{
	pid_t		child;

	if ((child = fork()) < 0)
		return (127);
	if (child == 0)
		lauch_child(shell, path);
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGCHLD, sig_ignore);
		waitpid(child, &g_stop, 0);
		signal(SIGINT, sig_ctrl_c);
		if (WIFSIGNALED(g_stop))
		{
			if (g_stop == 2)
				return (error_child("\n", 1));
			else if (g_stop == 3)
				return (error_child("Quit: 3\n", 131));
		}
		else
			return (WEXITSTATUS(g_stop));
	}
	return (0);
}

int		executable(t_shell *shell)
{
	char			*path;
	int				code;
	struct stat		sb;

	if ((path = getpath(shell)) == NULL)
	{
		if (!ft_strchr(shell->tab[0], '/'))
			return (127);
		if (-1 == stat(shell->tab[0], &sb))
			return (127);
		else if ((!S_ISREG(sb.st_mode) && !S_ISLNK(sb.st_mode)))
			return (126);
		else if (!(path = ft_strdup(shell->tab[0])))
			return (127);
	}
	if (ft_lstsize(shell->pipeline) == 1)
		code = launch_exec(shell, path);
	else
		code = change_exec(shell, path);
	ft_freez((void**)&path);
	return (code);
}
