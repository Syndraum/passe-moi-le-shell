/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 22:11:18 by mchardin          #+#    #+#             */
/*   Updated: 2020/02/13 21:21:35 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			command_pwd(t_shell *shell)
{
	shell->output = ft_strjoin(shell->pwd, "\n");
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
	while (shell->env_keys[++i])
		if (!ft_strncmp("CDPATH", shell->env_keys[i], 7) && shell->env_items[i]
			&& !(cd_paths = ft_split(shell->env_items[i], ':')))
			exit (1); //FREE A FAIRE
	i = -1;
	while (cd_paths[++i])
	{
		new = cd_paths[i][ft_strlen(cd_paths[i])] == '/' ?
		ft_sprintf("%s%s", cd_paths[i], dir)
		: ft_sprintf("%s/%s", cd_paths[i], dir);
		if (chdir(new) >= 0)
		{
			ft_free_strs(cd_paths);
			free(new);
			return (1);
		}
		free(new);
	}
	ft_free_strs(cd_paths);
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
	return (0);
}

// char		*print_export(char *var)
// {
// 	char	*ret;
// 	int		equal;
// 	char	*tmp;
//
// 	equal = ft_len_c(var, '=');
// 	ret = 0;
// 	ret = ft_strjoin_gnl(ret, "declare -x ");
// 	tmp = ft_substr(var, 0, equal + 1);
// 	ret = ft_strjoin_gnl(ret, tmp);
// 	free(tmp);
// 	ret = ft_strjoin_gnl(ret, "\"");
// 	ret = ft_strjoin_gnl(ret, &var[equal + 1]);
// 	ret = ft_strjoin_gnl(ret, "\"\n");
// 	// exit if malloc fail
// 	return (ret);
// }

// char		*print_export(char *var)
// {
// 	char	*ret;
// 	char	*beg;

// 	if (!(ret = malloc(sizeof(char) * (ft_strlen(var) + 15))))
// 		exit (1); // FREE
// 	beg = ret;
// 	ft_strlcpy(ret, "declare -x ", 12);
// 	ret = &ret[11];
// 	while (*var && *(var - 1) != '=')
// 		*ret++ = *var++;
// 	// ft_printf("%s\n", beg);
// 	*ret++ = '\"';
// 	while (*var)
// 		*ret++ = *var++;
// 	*ret++ = '\"';
// 	*ret++ = '\n';
// 	*ret = 0;
// 	return (beg);
// }

char		*print_export(char **keys, char **items)
{
	int		i;
	char	*output;
	char	*tmp;

	i = 0;
	while (keys[i])
	{
		if (ft_strncmp("_", keys[i], 2))
		{
			if (items[i])
				tmp = ft_sprintf("declare -x %s=\"%s\"\n", keys[i], items[i]);
			else
				tmp = ft_sprintf("declare -x %s\n", keys[i]);
			if (!(output = ft_strjoin_gnl(output, tmp)))
				exit(1); //free tmp
			free(tmp);
		}
		i++;
	}
	return (output);
}

int			command_export(t_shell *shell)
{
	int		i;

	if (shell->tab[1] == 0)
	{
		shell->output = print_export(shell->env_keys, shell->env_items);
		return (0);
	}
	i = 1;
	while (shell->tab[i])
	{
		if (is_var_def(shell->tab[i]) &&
		!replace_or_add(&shell->env_keys, &shell->env_items, shell->tab[i]))
			return (1);
		i++;
	}
	return (0);
}

int			command_unset(t_shell *shell)
{
	int		i;

	i = 1;
	while (shell->tab[i])
	{
		unset_var(shell->env_keys, shell->env_items, shell->tab[i]);
		i++;
	}
	return (0);
}

int			command_env(t_shell *shell) //JE RAPPELLE QUE LE SUJET NE DEMANDE PAS DE GERER LES ARGUMENTS
{
	int		i;

	i = 0;
	while (shell->env_keys[i])
	{
		if (shell->env_items[i] &&
			!(shell->output = ft_strjoin_gnl(shell->output,
			ft_sprintf("%s=%s\n", shell->env_keys[i], shell->env_items[i]))))
			return (1); //error
		i++;
	}
	return (0);
}
