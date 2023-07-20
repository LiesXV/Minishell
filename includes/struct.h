/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:12:58 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/07/20 12:08:24 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H
# include "./minishell.h"

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
	char			*var;
	char			*var_val;
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
}				t_data;

#endif