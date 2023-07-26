/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:51:29 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/07/26 06:27:32 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H
# include "./minishell.h"

int		do_nothing(char *input);
int		invalid_input(char *input, int i, char c);
int		input_handling(char *input, t_data *data);
char	**ft_multisplit(char *str, char *s);
int		contains(char c, char *s);
int		parse(t_parse *elem);
t_parse	**formating(char **cmds);
int	var_handler(t_parse *elem, int isarg, int nb, int keep_space);
int	redir(t_parse *elem, int i);
int var_redir(t_parse *elem, int dir, int space);
int quotes_error(char c);

#endif