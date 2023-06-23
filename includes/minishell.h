/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 15:09:28 by lmorel            #+#    #+#             */
/*   Updated: 2023/06/24 00:17:46 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../libft/libft.h"
# include <stdio.h>
# include <signal.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# define PROMPT "\033[32mThe \033[0;33mGolden\033[32m Minishell ~|>\033[0m "
# define SUCCESS 0
# define FAILURE 1

enum	e_type
{
	CMD = 0,
	PIPE = 1,
	REDIRECT = 2
};

/*
typedef struct s_list
{
	char			**cmd;
	char			*path;
	struct s_list	*next;
}					t_list; 
*/

typedef struct s_piplist
{
	char				*cmd;
	struct	s_piplist	*next;
}				t_piplist;

typedef struct s_parse
{
	int				i;
	int				j;
	enum e_type		t;
	char			*arg;
	char			**tmp;
	char			**args;
	char			*fullcmd;
	char			*cmd;
	char			*path;
	t_piplist		**piplist;
	struct	s_parse	*next;
}				t_parse;

typedef struct s_collector
{
	void				*address;
	struct s_collector	*next;
}				t_collector;

typedef struct s_data
{
	t_collector			*collector;
	t_parse				**cmd_lst;
	char				*path;
	char				**envp;
}				t_data;

t_list	*add_cmd(char *cmd, t_data *data);

int		add_address(t_collector **lst, void *address);
void	free_all(t_collector **lst);

char	*get_path(char *cmd, t_data *data);
int		get_cmd(t_parse *lst, t_data *data);

void    handle_exec(t_data *data);
int		is_builtin(char *cmd, t_data *data);
void	built_exit(t_data *data, char **args);

int		input_handling(char *input, t_data *data);
char	**ft_multisplit(char *str, char *s);
int	contains(char c, char *s);
int	parse(t_parse *elem);
t_parse	**formating(char **cmds);

#endif