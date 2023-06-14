# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/04 13:10:41 by ibenhaim          #+#    #+#              #
#    Updated: 2023/06/14 15:13:56 by lmorel           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	Minishell
FLAGS		=	-Wall -Werror -Wextra -pthread -fsanitize=address -g3

HEADER		=	${INCLUDES}minishell.h
SRCS		=	${SOURCES}main.c

INCLUDES	=	includes/
SOURCES		=	sources/


OBJS		= 	${SRCS:.c=.o}


%.o:%.c ${HEADER}
				${CC} ${FLAGS} -c $< -o $@

${NAME}		: ${OBJS} ${HEADER}
				${CC} ${FLAGS} ${OBJS} -o ${NAME}

all : ${NAME}

clean:
				rm -f ${OBJS} 

fclean:	clean
				rm -f ${NAME}

re: fclean all

.PHONY:	all clean fclean re