SRCS			=	srcs/main.c	\
					srcs/init.c	\
					srcs/row.c	\
					srcs/col.c	\
					srcs/square.c	\
					srcs/bruteForce.c	\
					srcs/thread.c	\
					srcs/utils.c	\
					srcs/free.c	\
					srcs/print.c

OBJS			= $(SRCS:.c=.o)

CC				= gcc

FLAGS			= -Wall -Wextra -Werror -I sudoku.h

RM				= rm -f

NAME			= sudoku

all:			$(NAME)

$(NAME):		$(OBJS)
				$(CC) $(FLAGS) $(OBJS) -o $(NAME)


clean:
				$(RM) $(OBJS)

fclean:			clean
				$(RM) $(NAME)

re:				fclean $(NAME)