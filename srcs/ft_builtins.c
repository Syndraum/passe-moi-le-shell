/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 22:11:18 by mchardin          #+#    #+#             */
/*   Updated: 2020/02/06 09:42:33 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			command_pwd(t_shell *shell)
{
	shell->output = ft_strjoin(shell->pwd, "\n");
	return (0);
}

char		*home_path(char **environ)
{
	int		i;

	i = 0;
	while (environ[i])
	{
		if (is_var_to_unset("HOME", environ[i]))
			return (&environ[i][5]);
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
	while (shell->environ[++i])
		if (is_var_to_unset("CDPATH", shell->environ[i])
			&& !(cd_paths = ft_split(&shell->environ[i][7], ':')))
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
		&& !(buf = home_path(shell->environ)))
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
	free(shell->pwd);
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

char		*print_export(char *var)
{
	char	*ret;
	char	*beg;

	if (!(ret = malloc(sizeof(char) * (ft_strlen(var) + 15))))
		exit (1); // FREE
	beg = ret;
	ft_strlcpy(ret, "declare -x ", 12);
	ret = &ret[11];
	while (*var && *(var - 1) != '=')
		*ret++ = *var++;
	// ft_printf("%s\n", beg);
	*ret++ = '\"';
	while (*var)
		*ret++ = *var++;
	*ret++ = '\"';
	*ret++ = '\n';
	*ret = 0;
	return (beg);
}

int			command_export(t_shell *shell)
{
	int		i;
	char	*tmp;

	if (shell->tab[1] == 0)
	{
		i = 0;
		while (shell->environ[i])
		{
			if (!is_var_to_unset("_", shell->environ[i]))
			{
				tmp = print_export(shell->environ[i]);
				if (!(shell->output = ft_strjoin_gnl(shell->output, tmp)))
					exit(1); //free tmp
				free(tmp);
			}
			i++;
		}
		return (0);
	}
	i = 1;
	while (shell->tab[i])
	{
		if (is_var_def(shell->tab[i]) &&
		!replace_or_add(&shell->environ, shell->tab[i]))
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
		unset_var(shell->environ, shell->tab[i]);
		i++;
	}
	return (0);
}

int			command_env(t_shell *shell) //JE RAPPELLE QUE LE SUJET NE DEMANDE PAS DE GERER LES ARGUMENTS
{
	int		i;

	i = 0;
	while (shell->environ[i])
	{
		if (!(shell->output = ft_strjoin_gnl(shell->output, shell->environ[i]))
			|| !(shell->output = ft_strjoin_gnl(shell->output, "\n")))
			exit (1); //FREE
		i++;
	}
	return (0);
}
