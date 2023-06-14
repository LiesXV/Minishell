/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 15:09:28 by lmorel            #+#    #+#             */
/*   Updated: 2023/06/14 19:28:19 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../libft/libft.h"
# include <stdio.h>
# include <signal.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# define PROMPT "\033[32mThe Golden Minishell ~|>\033[0m"
# define SUCCESS 0
# define FAILURE 1

typedef struct s_collector
{
	void				*address;
	struct s_collector	*next;
}				t_collector;

typedef struct s_data
{
	t_collector			*collector;
	t_list				*cmd_lst;
	char				*path;
	char				**envp;
}				t_data;

t_list	*add_cmd(char *cmd, t_data *data);

int		add_address(t_collector **lst, void *address);
void	free_all(t_collector **lst);

int		get_cmd(t_list	*lst, t_data *data);

char	*get_path(char *cmd, t_data *data);

int	input_handling(char *input, t_data *data);

#endif