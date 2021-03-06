
NAME 			=	pipex

HEADER			=	./pipex.h

SRCS			=	main.c utils.c

OBJS			= $(SRCS:.c=.o)
%.o:			%.c ${HEADER}
				$(CC) $(CFLAGS) -c -o $@ $<

RM				= rm -f
CFLAGS			= -Wall -Werror -Wextra
INCLUDES		= 

$(NAME):		$(OBJS) $(HEADER)
				$(MAKE) -C ./libft
				gcc -g $(OBJS) ./libft/libft.a -o ${NAME}

all:			$(NAME)
clean:
				$(MAKE) clean -C ./libft	
				$(RM) $(OBJS)		

fclean:			clean
				$(MAKE) fclean -C ./libft
				$(RM) $(NAME)
				
re:				fclean all
test:			all
.PHONY:			all clean fclean re
