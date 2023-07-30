/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:51:29 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/07/29 23:30:30 by lmorel           ###   ########.fr       */
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

int		do_nothing(char *input);
int		only_spaces(char *str);
int		invalid_input(char *input, int i, char c);
int		input_handling(char *input, t_data *data);
char	**ft_multisplit(char *str, char *s);
int		contains(char c, char *s);
int		parse(t_parse *elem);
t_parse	**formating(char **cmds);
int	var_handler(t_parse *elem, int isarg, int nb, int keep_space);
int	redir(t_parse *elem, int i);
int var_redir(t_parse *elem, int dir, int space);
void	handle_pipes(t_parse *elem);
int	error_in_quotes(char *input, int i);
int quotes_error(char c);
char	**trixsplit(char const *s, char c);

#endif