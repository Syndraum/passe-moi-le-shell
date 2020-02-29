/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 22:11:18 by mchardin          #+#    #+#             */
/*   Updated: 2020/02/29 16:48:08 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			command_pwd(t_shell *shell)
{
	if (!(shell->output = ft_strjoin(shell->pwd, "\n")))
		exit_error(shell, "pwd");
	return (0);
}

char		*home_path(char **keys, char **items)
{
	int		i;

	i = 0;
	while (keys[i])
	{
		if (!ft_strncmp("HOME", keys[i], 5))
			return (items[i]);
		i++;
	}
	ft_printf("minishell: cd: HOME not set\n");
	return (0);
}

int			cd_path(t_shell *shell, char *dir)
{
	char		**cd_paths;
	int			i;
	char		*new;

	i = -1;
	cd_paths = 0;
	while (shell->env_keys[++i])
	{
		if (!ft_strncmp("CDPATH", shell->env_keys[i], 7) && shell->env_items[i]
		&& !(cd_paths = ft_split(shell->env_items[i], ':')))
			exit_error(shell, "cd"); //FREE A FAIRE
	}
	i = -1;
	if (cd_paths)
	{
		while (cd_paths[++i])
		{
			new = cd_paths[i][ft_strlen(cd_paths[i])] == '/' ?
			ft_sprintf("%s%s", cd_paths[i], dir)
			: ft_sprintf("%s/%s", cd_paths[i], dir);
			if (!new)
			{
				ft_free_strs(cd_paths);
				exit_error(shell, "cd");
			}
			if (chdir(new) >= 0)
			{
				ft_free_strs(cd_paths);
				shell->output = ft_strjoin_gnl(new, "\n");
				return (1);
			}
			ft_freez(new);
		}
	ft_free_strs(cd_paths);
	}
	return (0);
}

int			command_cd(t_shell *shell)
{
	char	*buf;

	buf = shell->tab[1];
	if ((buf == 0 || (ft_strlen(buf) == 1 && ft_strncmp(buf, "~", 1) == 0))
	&& !(buf = home_path(shell->env_keys, shell->env_items)))
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
		ft_dprintf(2, "minishell: cd: %s : %s\n", //
			shell->tab[1], strerror(errno)); //
		return (1); // error message?
	}
	shell->oldpwd = shell->pwd;
	shell->pwd = buf;
	pwd_env(shell);
	return (0); //everything went well
}

char		*print_export(char **keys, char **items)
{
	int		i;
	char	*output;
	char	*tmp;

	i = 0;
	output = 0;
	while (keys[i])
	{
		if (items[i])
			tmp = ft_sprintf("declare -x %s=\"%s\"\n", keys[i], items[i]);
		else
			tmp = ft_sprintf("declare -x %s\n", keys[i]);
		if (!tmp)
			return (0);
		output = ft_strjoin_gnl(output, tmp);
		ft_freez(tmp);
		i++;
	}
	return (output);
}

int			command_export(t_shell *shell)
{
	int		i;
	char	*key;
	char	*item;
	int		ret;

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
			return (1); // wrong identifier?
		i++;
	}
	return (0);
}

int			command_unset(t_shell *shell)
{
	int		i;
	char	*key;
	char	*item;
	int		ret;

	i = 1;
	while (shell->tab[i])
	{
		if (!(ret = check_split_var(shell->tab[i], &key, &item) && !item))
			unset_var(shell->env_keys, shell->env_items, key);
		else if (ret < 0)
			exit_error(shell, "unset");
		else
			ft_freez(item);
		ft_freez(key);
		i++;
	}
	return (0);
}

int			command_env(t_shell *shell) //JE RAPPELLE QUE LE SUJET NE DEMANDE PAS DE GERER LES ARGUMENTS
{
	int		i;
	char	*tmp;

	i = 0;
	while (shell->env_keys[i])
	{
		if (shell->env_items[i])
		{
			if (!(tmp = ft_sprintf("%s=%s\n", shell->env_keys[i], shell->env_items[i]))
			|| !(shell->output = ft_strjoin_gnl(shell->output, tmp)))
				exit_error(shell, "env");
		}
		i++;
	}
	return (0);
}
