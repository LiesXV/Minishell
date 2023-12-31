/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 15:09:28 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/25 16:56:26 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../libft/libft.h"
# include <stdio.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <stdlib.h>
# include <stdbool.h>
# include <fcntl.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# define PROMPT "\001\033[32m\002The \001\033[0;33m\002\
Golden\001\033[32m\002 Minishell ~|>\001\033[0m\002 "
# define SUCCESS 0
# define FAILURE 1
# include "struct.h"
# include "builtins.h"
# include "exec.h"
# include "parsing.h"

extern int	g_end_status;

int		init_signals(int token);
int		add_address(t_collector **lst, void *address);
void	free_all(t_collector **lst);

# define DEBUG 0

#endif