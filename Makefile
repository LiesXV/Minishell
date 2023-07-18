# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/04 13:10:41 by ibenhaim          #+#    #+#              #
#    Updated: 2023/07/18 03:12:30 by ibenhaim         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell
FLAGS		=	-Wall -Werror -Wextra -fsanitize=address -g3

HEADER		=	minishell.h builtins.h struct.h

SRCS		=	main.c garbage_collector.c get_cmd.c add_cmd.c is_command.c input_handling.c \
				built_exit.c builtins.c ft_multisplit.c parse.c formating.c handle_exec.c \
				built_echo.c built_pwd.c built_cd.c handle_vars.c

INCLUDES_DIR	=	includes/
SRCS_DIR		=	srcs/
LIBFT			=	libft/

HEADER_FILES	=	$(addprefix $(INCLUDES_DIR), $(HEADER))
SRCS_FILES		=	$(addprefix $(SRCS_DIR), $(SRCS))
OBJS			= 	${SRCS_FILES:.c=.o}
LFTNAME			= 	libft.a

%.o:%.c ${HEADER_FILES} $(LIBFT)*.c $(LIBFT)libft.h
				${CC} ${FLAGS}  -c $< -o $@

${NAME}		: ${OBJS} ${HEADER_FILES}
				make -C libft
				${CC} ${FLAGS} ${OBJS} ${LIBFT}${LFTNAME} -o ${NAME} -lreadline

all : ${NAME}

clean:
				rm -f ${OBJS}
				${MAKE} -C ${LIBFT} clean

fclean:	clean
				rm -f ${NAME}
				${MAKE} -C ${LIBFT} fclean

re: fclean all

.PHONY:	all clean fclean re
