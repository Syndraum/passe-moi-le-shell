PREFIX = srcs/

SRCS = $(addprefix $(PREFIX), main.c ft_arg.c ft_echo.c)

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

fclean:	clean

re:	fclean all