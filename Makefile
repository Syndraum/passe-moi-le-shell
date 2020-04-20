PREFIX = srcs/

SRCS = $(addprefix $(PREFIX), 	ft_minishell.c \
								ft_init.c \
								ft_init_env.c \
								ft_is_var.c \
								ft_builtins.c \
								ft_builtins_utils.c \
								ft_env_utils.c \
								ft_arg_interpretation.c \
								ft_arg_interpretation_utils.c \
								ft_arg_translation.c \
								ft_arg_translation_utils.c \
								ft_arg_dquote.c \
								ft_arg_unquote.c \
								ft_cursor_utils.c \
								ft_echo.c \
								ft_executable.c \
								ft_executable_utils.c \
								ft_path.c \
								ft_exit.c \
								ft_pipes_n_signals.c \
								ft_free_close.c \
								ft_find_env.c \
								ft_arg_dollar.c \
								ft_command.c \
								ft_main_loop.c)

OBJS = ${SRCS:.c=.o}

INCLUDES	=	./includes/

INCLUDES_L	=	./libft/includes/

CC	= clang

CFLAGS = -g -Werror -Wextra -Wall -I ${INCLUDES} -I ${INCLUDES_L}

NAME 	= minishell

RM		= rm -f

DIR_LIBFT	= libft

LIBFT_FLAGS	= -L./libft/ -lft

all:	${NAME}

.c.o:
	${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}:	makelib ${OBJS}
	${CC} ${CFLAGS} ${OBJS} ${LIBFT_FLAGS} -o ${NAME}

run:	all
	./$(NAME)

makelib:
	make -C ${DIR_LIBFT}

clean:
	${RM} ${OBJS}
	make -C ${DIR_LIBFT} clean

fclean:	clean
	make -C ${DIR_LIBFT} fclean
	${RM} ${NAME}

re:	fclean all