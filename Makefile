# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/04 13:10:41 by ibenhaim          #+#    #+#              #
#    Updated: 2023/06/20 19:04:23 by lmorel           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell
CC			=	gcc
FLAGS		=	-Wall -Werror -Wextra -lreadline -fsanitize=address -g3

HEADER		=	${INCLUDES}minishell.h
SRCS		=	${SOURCES}main.c ${SOURCES}garbage_collector.c ${SOURCES}get_cmd.c ${SOURCES}add_cmd.c ${SOURCES}is_command.c ${SOURCES}input_handling.c \
				${SOURCES}built_exit.c ${SOURCES}builtins.c ${SOURCES}ft_multisplit.c  ${SOURCES}parse.c ${SOURCES}formating.c

INCLUDES	=	includes/
SOURCES		=	srcs/
LIBFT		=	libft/


OBJS		= 	${SRCS:.c=.o}
LFTNAME		= 	libft.a

%.o:%.c ${HEADER} $(LIBFT)*.c $(LIBFT)libft.h
				${CC} ${FLAGS} -c $< -o $@

${NAME}		: ${OBJS} ${HEADER}
				make -C libft
				${CC} ${FLAGS} ${OBJS} ${LIBFT}${LFTNAME} -o ${NAME}

all : ${NAME}

clean:
				rm -f ${OBJS}
				${MAKE} -C ${LIBFT} clean

fclean:	clean
				rm -f ${NAME}
				${MAKE} -C ${LIBFT} fclean

re: fclean all

.PHONY:	all clean fclean re