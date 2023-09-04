/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:12:58 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/04 14:26:11 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H
# include "./minishell.h"

/*
typedef struct s_list
{
	char			**cmd;
	char			*path;
	struct s_list	*next;
}					t_list; 
*/

struct s_data;

typedef struct s_redir
{
	char			*out1;
	char			*out2;
	char			*in;
	int				sstdin;
	int				sstdout;
	int				sstderr;
	int				end;
	int				i;
	char			**hd;
	struct s_redir	*next;
}				t_redir;

typedef struct s_piplist
{
	char				*path;
	char				**cmd;
	t_redir				redir;
	struct s_piplist	*next;
}				t_piplist;

typedef struct s_parse
{
	int				i;
	int				j;
	char			*arg;
	char			**tmp;
	char			**args;
	char			*fullcmd;
	char			*cmd;
	char			*path;
	t_piplist		**piplist;
	t_redir			**rlist;
	t_redir			redir;
	char			*var;
	char			*var_val;
	struct s_data	*p_data;
	struct s_parse	*next;
}				t_parse;

typedef struct s_collector
{
	void				*address;
	struct s_collector	*next;
}				t_collector;

typedef struct s_env
{
	char			*var_name;
	char			*var_content;
	struct s_env	*next;
}				t_env;

typedef struct s_data
{
	t_collector			*collector;
	t_parse				**cmd_lst;
	t_env				*env;
	char				*path;
	char				**envp;
	int					old_fd[2];
	int					new_fd[2];
	int					infile;
	int					outfile;
}				t_data;

#endif