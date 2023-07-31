/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 15:09:28 by lmorel            #+#    #+#             */
/*   Updated: 2023/07/31 21:58:38 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../libft/libft.h"
# include <stdio.h>
# include <signal.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <stdbool.h>
# include <fcntl.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# define PROMPT "\033[32mThe \033[0;33mGolden\033[32m Minishell ~|>\033[0m "
# define SUCCESS 0
# define FAILURE 1
# include "struct.h"
# include "builtins.h"
# include "exec.h"
# include "parsing.h"

int		add_address(t_collector **lst, void *address);
void	free_all(t_collector **lst);

extern int	g_end_status;

# define DEBUG 1

#endif