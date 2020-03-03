/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/03 14:19:15 by mchardin          #+#    #+#             */
/*   Updated: 2020/03/03 19:29:04 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		*ft_shlvl(t_shell *shell)
{
	int		idx;
	int		ret;
	int		j;

	j = 1;
	idx = get_tabidx("SHLVL", shell->env_keys);
	if (idx >= 0)
	{
		if (!(ft_isdigit(shell->env_items[idx][0])
			|| shell->env_items[idx][0] == '+'
			|| shell->env_items[idx][0] == '-'))
			return (ft_strdup("1"));
		while (shell->env_items[idx][j])
		{
			if (!(ft_isdigit(shell->env_items[idx][j])))
				return (ft_strdup("1"));
			j++;
		}
		ret = ft_atoi(shell->env_items[idx]);
		return (ret < 0 ? ft_strdup("0") : ft_itoa(ret + 1));
	}
	return (ft_strdup("1"));
}

void		ft_open_line(char *file, t_shell *shell)
{
	if ((shell->fd_line = open(file, O_RDONLY)) < 0)
	{
		ft_printf("minishell: %s: %s\n", file, strerror(errno));
		exit(1);
	}
}

void		ft_first_init_struct(t_shell *shell)
{
	shell->env_keys = 0;
	shell->env_items = 0;
	shell->arg.str = 0;
	shell->output = 0;
	shell->pwd = 0;
	shell->oldpwd = 0;
	shell->pipeline = 0;
	shell->tab = 0;
	shell->line[0] = 0;
	shell->cursor[0] = 0;
	shell->fd_line = 0;
	shell->fd_input = 0;
	shell->fd_output = 1;
}

void		ft_mainargs(int argc, char **argv, char **envp, t_shell *shell)
{
	char	*buf;
	char	*tmp;

	ft_first_init_struct(shell);
	if (argc > 1)
		ft_open_line(argv[1], shell);
	ft_env_lib(shell, envp);
	buf = 0;
	if (!(buf = getcwd(buf, 0)))
		exit_error(shell, 0);
	shell->pwd = buf;
	if (!(tmp = ft_strdup(buf))
		|| !replace_or_add(&shell->env_keys, &shell->env_items,
		ft_strdup("PWD"), tmp)
		|| !(tmp = ft_shlvl(shell))
		|| !replace_or_add(&shell->env_keys, &shell->env_items,
		ft_strdup("SHLVL"), tmp)
		|| !init_oldpwd(&shell->env_keys, &shell->env_items))
		exit_error(shell, 0);
}
