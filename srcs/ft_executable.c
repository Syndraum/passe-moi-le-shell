/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executable.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/02 13:35:47 by mchardin          #+#    #+#             */
/*   Updated: 2020/04/14 15:06:32 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*getpath(t_shell *shell)
{
	char			**tab;
	int				i;
	char			*path;
	struct stat		sb;

	i = -1;
	if (!(tab = ft_split(get_item("PATH", shell->env_keys, shell->env_items), ':')))
		return (NULL);
	while (tab[++i])
	{
		if ((path = try_path(shell->tab[0], tab[i])))
		{
			ft_free_strs(&tab);
			return (path);
		}
	}
	ft_free_strs(&tab);
	if (!(path = ft_strdup(shell->tab[0])))
		return (NULL);
	if (-1 == stat(path, &sb) || (!S_ISREG(sb.st_mode) && !S_ISLNK(sb.st_mode)))
	{
		free(path);
		return (NULL);
	}
	return (path);
}

int		change_exec(t_shell *shell, char *path)
{
	char		**env;

	if (!(env = convert_env(shell, path)))
		return (1);
	if (0 > execve(path, shell->tab, env))
		return (1);
	return (0);
}

int		launch_exec(t_shell *shell, char *path)
{
	pid_t		child;

	if ((child = fork()) < 0)
		return (127);
	if (child == 0)
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
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGCHLD, sig_ignore);
		waitpid(child, &shell->stop, 0);
		signal(SIGINT, sig_ctrl_c);
		if (WIFSIGNALED(shell->stop))
		{
			if (shell->stop == 2)
			{
				ft_putchar_fd('\n', 0);
				return (1);
			}
			else if (shell->stop == 3)
			{
				ft_putstr_fd("Quit: 3\n", 0);
				return (131);
			}
		}
		else
			return (WEXITSTATUS(shell->stop));
	}
	return (0);
}

int		executable(t_shell *shell)
{
	char		*path;
	int			code;

	if ((path = getpath(shell)) == NULL)
	{
		if (ft_strchr(shell->tab[0], '/'))
			return (126);
		return (127);
	}
	if (ft_lstsize(shell->pipeline) == 1)
		code = launch_exec(shell, path);
	else
		code = change_exec(shell, path);
	ft_freez((void**)&path);
	return (code);
}
