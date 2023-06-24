/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:51:29 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/06/24 15:51:29 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H
# include "./minishell.h"

int		input_handling(char *input, t_data *data);
char	**ft_multisplit(char *str, char *s);
int		contains(char c, char *s);
int		parse(t_parse *elem);
t_parse	**formating(char **cmds);

#endif