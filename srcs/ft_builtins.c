/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 22:11:18 by mchardin          #+#    #+#             */
/*   Updated: 2020/03/07 21:12:00 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			command_pwd(t_shell *shell)
{
	if (!(shell->output = ft_strjoin(shell->pwd, "\n")))
		exit_error(shell, "pwd");
	return (0);
}

int			command_cd(t_shell *shell)
{
	char	*buf;

	buf = shell->tab[1];
	if (!buf && !(buf = home_path(shell->env_keys, shell->env_items)))
		return (1);
	if (chdir(buf) < 0 && (buf[0] == '/' || !cd_path(shell, buf)))
	{
		ft_dprintf(2, "minishell: cd: %s : %s\n",
			shell->tab[1], strerror(errno));
		return (1);
	}
	buf = 0;
	if (!(buf = getcwd(buf, 0)))
	{
		ft_dprintf(2, "minishell: cd: %s : %s\n",
			shell->tab[1], strerror(errno));
		return (1);
	}
	ft_freez((void**)&shell->oldpwd);
	shell->oldpwd = shell->pwd;
	shell->pwd = buf;
	pwd_env(shell);
	return (0);
}

int			command_export(t_shell *shell)
{
	int		i;
	char	*key;
	char	*item;
	int		ret;
	int		err;

	err = 0;
	if (!shell->tab[1])
	{
		if (!(shell->output = print_export(shell->env_keys, shell->env_items)))
			exit_error(shell, "export");
		return (0);
	}
	i = 1;
	while (shell->tab[i])
	{
		if ((ret = check_split_var(shell->tab[i], &key, &item)) < 0 || (!ret
		&& !replace_or_add(&shell->env_keys, &shell->env_items, key, item)))
			exit_error(shell, "export");
		else if (ret > 0)
			err += ft_dprintf(2, "minishell: export: `%s': %s\n",
			shell->tab[i], ERR_ID);
		i++;
	}
	return (err > 0 ? 1 : 0);
}

int			command_unset(t_shell *shell)
{
	int		i;
	char	*key;
	char	*item;
	int		ret;
	int		err;

	i = 1;
	err = 0;
	while (shell->tab[i])
	{
		if ((ret = check_split_var(shell->tab[i], &key, &item)) < 0 ||
		(!ret && !item && !unset_var(shell->env_keys, shell->env_items, key)))
			exit_error(shell, "unset");
		else if (ret > 0 || item)
		{
			err += ft_dprintf(2, "minishell: unset: `%s': %s\n",
			shell->tab[i], ERR_ID);
		}
		ft_freez((void **)&item);
		ft_freez((void **)&key);
		i++;
	}
	return (err > 0 ? 1 : 0);
}

int			command_env(t_shell *shell) //JE RAPPELLE QUE LE SUJET NE DEMANDE PAS DE GERER LES ARGUMENTS
{
	int		i;
	char	*tmp;

	i = 0;
	while (shell->env_keys[i])
	{
		if (shell->env_items[i] && ft_strncmp(shell->env_keys[i], "_", 2))
		{
			if (!(tmp = ft_sprintf("%s=%s\n",
				shell->env_keys[i], shell->env_items[i]))
			|| !(shell->output = ft_strjoin_gnl(shell->output, tmp)))
			{
				ft_freez((void**)&tmp);
				exit_error(shell, "env");
			}
		}
		ft_freez((void**)&tmp);
		i++;
	}
	if (!(shell->output = ft_strjoin_gnl(shell->output, ENV_COMMAND)))
		exit_error(shell, "env");
	return (0);
}
