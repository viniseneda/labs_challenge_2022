SRCS	= log_reader.c fort.c tables.c parsing.c utils.c
OBJS	= ${SRCS:.c=.o}

CC	= cc

CFLAGS = -g3 #-Wall -Werror -Wextra -g3

NAME	= log_reader

.c.o:
	make -C ./libft all
	${CC} ${CFLAGS} -c ${SRCS}

RM	= rm -f

$(NAME):	${OBJS}
	gcc -g -o $(NAME) ${OBJS} -L./libft -lft


all:	$(NAME)

clean:
	${RM} ${OBJS}
	make -C ./libft clean

fclean:	clean
	${RM} ${NAME}
	make -C ./libft clean

re:	fclean all

.PHONY: 	all clean fclean
