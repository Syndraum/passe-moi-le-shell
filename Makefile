PREFIX = srcs/

SRCS = $(addprefix $(PREFIX), main.c ft_is_var.c ft_command_utils.c ft_command.c ft_arg.c ft_arg2.c ft_echo.c)

OBJS = ${SRCS:.c=.o}

INCLUDES	=	./includes/

INCLUDES_L	=	./libft/includes/

CC	= gcc

CFLAGS = -g -Werror -Wextra -Wall -I ${INCLUDES} -I ${INCLUDES_L}

NAME 	= minishell

RM		= rm -f

DIR_LIBFT	= libft

LIBFT_FLAGS	= -L./libft/ -lft

all:	${NAME}

.c.o:
	${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}:	makelib ${OBJS}
	${CC} ${CFLAGS} ${LIBFT_FLAGS} ${OBJS} -o ${NAME}

makelib:
	make -C ${DIR_LIBFT}

clean:
	${RM} ${OBJS}
	make -C ${DIR_LIBFT} clean

fclean:	clean
	make -C ${DIR_LIBFT} fclean
	${RM} ${NAME}

re:	fclean all