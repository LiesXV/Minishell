# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/04 13:10:41 by ibenhaim          #+#    #+#              #
#    Updated: 2023/07/26 05:42:13 by lmorel           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell
FLAGS		=	-Wall -Werror -Wextra -fsanitize=address -g3

HEADER		=	minishell.h builtins.h struct.h

SRCS		=	main.c garbage_collector.c exec/get_cmd.c exec/add_cmd.c exec/is_command.c parsing/input_handling.c \
				builtins/built_exit.c builtins/builtins.c parsing/ft_multisplit.c parsing/parse.c parsing/formating.c exec/handle_exec.c \
				builtins/built_echo.c builtins/built_env.c builtins/built_pwd.c builtins/built_cd.c parsing/handle_vars.c parsing/redir.c parsing/input_checking.c

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
