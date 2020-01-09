/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchardin <mchardin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/15 10:36:20 by mchardin          #+#    #+#             */
/*   Updated: 2019/11/17 14:58:54 by mchardin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>

typedef struct	s_buf
{
	int		r;
	char	buf[1][BUFFER_SIZE + 1];
}				t_buf;

# define B BUFFER_SIZE

int				get_next_line(int fd, char **line);
char			*ft_strjoin_gnl(char *s1, char *s2);

#endif
