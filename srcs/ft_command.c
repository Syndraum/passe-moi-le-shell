/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 22:11:18 by mchardin          #+#    #+#             */
/*   Updated: 2020/02/01 17:30:16 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		command_pwd(t_shell *shell)
{
	shell->output = ft_strjoin(shell->pwd, "\n");
}

int			command_cd(t_shell *shell)
{
	char	*buf;

	buf = 0;
	if (chdir(shell->tab[1]) < 0)
	{
		ft_dprintf(2, "minishell: cd: %s : %s\n",
			shell->tab[1], strerror(errno));
		return (0);
	}
	if (!(buf = getcwd(buf, 0)))
	{
		ft_dprintf(2, "minishell: cd: %s : %s\n", //
			shell->tab[1], strerror(errno)); //
		return (0); // error message?
	}
	free(shell->pwd);
	shell->pwd = buf;
	pwd_env(shell);
	return (1);
}

int			command_export(t_shell *shell)
{
	int		i;

	i = 1;
	while (shell->tab[i])
	{
		if (is_var_def(shell->tab[i]) &&
		!replace_or_add(shell->environ, shell->tab[i]))
			return (0);
		i++;
	}
	return (1);
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

void		command_env(t_shell *shell) //JE RAPPELLE QUE LE SUJET NE DEMANDE PAS DE GERER LES ARGUMENTS
{
	int		i;

	i = 0;
	while (shell->environ[i])
	{
		ft_putendl_fd(shell->environ[i], shell->fd);
		i++;
	}
}
