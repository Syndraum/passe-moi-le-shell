/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 22:11:18 by mchardin          #+#    #+#             */
/*   Updated: 2020/01/12 14:04:20 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		command_pwd(t_shell *shell)
{
	char	*buf;

	buf = 0;
	buf = getcwd(buf, 0);
	shell->output = ft_strjoin(buf, "\n");
	free(buf);
}

int			command_cd(t_shell *shell)
{
	if (chdir(shell->tab[1]) < 0)
	{
		ft_dprintf(2, "%s\n", strerror(errno));
		return (0);
	}
	return (1);
}

int			replace_var(char **environ, char *var)
{
	int		i;

	i = 0;
	while (environ[i])
	{
		if (is_same_var(var, environ[i]))
		{
			free(environ[i]);
			environ[i] = var;
			return (1);
		}
		i++;
	}
	return (0);
}

int			command_export(t_shell *shell)
{
	int		i;

	i = 1;
	while (shell->tab[i])
	{
		if (!(is_var_def(shell->tab[i])))
			return (0);
		i++;
	}
	i = 1;
	while (shell->tab[i])
	{
		if (!replace_var(shell->environ, shell->tab[i]) &&
			!(shell->environ = ft_strs_plus_one(shell->environ, shell->tab[i])))
		{
			ft_dprintf(2, "%s\n", strerror(errno));
			return (0);
		}
		i++;
	}
	return (1);
}

void			unset_var(char **environ, char *var)
{
	int		i;
	int		last;

	last = ft_strslen(environ) - 1;
	i = 0;
	while (environ[i])
	{
		if (is_var_to_unset(var, environ[i]))
		{
			free(environ[i]);
			environ[i] = environ[last];
			environ[last] = 0;
			return ;
		}
		i++;
	}
	ft_dprintf(2, "minishell: unset: `%s': not a valid identifier\n", var);
}

int			command_unset(t_shell *shell)
{
	int		i;

	i = 1;
	while (shell->tab[i])
	{
		unset_var(shell->environ, shell->tab[i]);
		i++;
	}
	return (1);
}

void		command_env(t_shell *shell)
{
	int		i;

	i = 0;
	while (shell->environ[i])
	{
		ft_putendl_fd(shell->environ[i], shell->fd);
		i++;
	}
}
