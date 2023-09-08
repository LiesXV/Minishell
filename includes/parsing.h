/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:51:29 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/08 19:52:01 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H
# include "./minishell.h"

typedef struct s_trix
{
	int		k;
	int		i;
	int		j;
	int		len;
	int		count_words;
	char	c;
	char	**str;
}				t_trix;

int		search_env(t_data *data, char *env);
char	*get_env_val(t_data *data, char *name);
int		do_nothing(char *input);
int		only_spaces(char *str);
int		invalid_input(char *input, int i, char c);
int		input_handling(char *input, t_data *data);
char	**ft_multisplit(char *str, char *s);
int		contains(char c, char *s);
int		parse(t_parse *elem);
t_parse	**formating(char **cmds, t_data *data);
int		var_handler(t_parse *elem, int isarg, int nb, int keep_space);
int		redir(t_parse *elem, int i);
int		var_redir(t_parse *elem, int dir, int space);
int		handle_pipes(t_parse *elem);
int		error_in_quotes(char *input, int i);
int		quotes_error(char c);
char	**trixsplit(char const *s, char c);
void	printlist(t_parse **head);
int		here_doc(t_parse *elem);
int		add_tab_to_gb(t_parse *elem, char **args);
void	rlist_add_back(t_redir **lst, t_redir *new);
t_redir	*new_rlist_elem(t_parse *elem);
void	redir_reset(t_parse *elem, int i);
int		ft_strstr_bool(const char *str, const char *find, size_t len);
void	free_tab(void **tab);
void	free_all_split(char **str, int index);
int		watch_protect(char const *s, int i, char c, t_trix *split);
void	put_words(char const *s, int words, t_trix *sp);

int		s_quote_redir(t_parse *elem, char *file);
int quote_util_redir(t_parse *elem, char *file, int std, int ret);
int d_quote_redir(t_parse *elem, char *file, int std);
int	redir_quote(t_parse *elem, int i, char *file);

int pipe_error(char c);
int quotes_error(char c);
int	error_exit(char *name, char *err);

char	*only_cmd(t_parse *elem);
int		cmd_redir(t_parse *elem, int ret);
int		quotes_handler(t_parse *elem);
int	double_quotes(t_parse *elem);
int		single_quotes(t_parse *elem);

int file_create(t_parse *elem, int type);
int	file_in_create(t_parse *elem);
int	file_out2_create(t_parse *elem);
t_redir *new_rlist_elem(t_parse *elem);
void	rlist_add_back(t_redir **lst, t_redir *new);

int	put_error_msg(char c, int i, char *input);
int	error_in_quotes(char *input, int i);
int	hd_utils(t_parse *elem, char *hd, int ret);

int	add_space_before_util(char *fullcmd, int count, char *tmp);
int	add(char *fullcmd, char *copy, char **new, int count);
int	after_var(int count, char *fullcmd);
int	test_value(t_parse *elem);
int	var_state(t_parse *elem, int isarg, int i);
int	init_var_str(t_parse *elem);
int	dollar_error(t_parse *elem, int isarg, int nb);
void	error_undef(char *name);
void	place_var(t_parse *elem, int isarg, int nb);
char	*handle_var_spaces(char *str, char *fullcmd, int count);
int	mid_space(char *str);
void	add_space_after(char *tmp, char *fullcmd, int count, char **new);
int	add_space_before(char *tmp, char *fullcmd, int count, char **new);
int	add_space_before_condition(char *fullcmd, int count);
int	arg_is_blank(char *arg, t_parse *elem);
int	free_tmp(t_parse *elem, int i);
int	init_parse_arg(t_parse *elem, int nb);
int	init_args(t_parse *elem, int nb, int j);
int		arg_redir(t_parse *elem, int err, int nb);
void	pip_add_back(t_piplist **lst, t_piplist *new);
t_redir	create_pip_redir(char *str, t_parse *elem);
int		valid_pip(char c, char *fullcmd);

#endif