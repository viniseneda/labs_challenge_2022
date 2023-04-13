SRCS	= print_json.c mjson.c routes.c sql_actions.c api.c mongoose.c cJSON.c
OBJS	= ${SRCS:.c=.o}

CC	= cc

CFLAGS =  #-Wall -Werror -Wextra

NAME	= api

.c.o:
	${CC} ${CFLAGS} -c ${SRCS}

RM	= rm -f

$(NAME):	${OBJS}
	cc -o $(NAME) ${OBJS} -g -l sqlite3


all:	$(NAME)

clean:
	${RM} ${OBJS}

fclean:	clean
	${RM} ${NAME}

re:	fclean all

.PHONY: 	all clean fclean
