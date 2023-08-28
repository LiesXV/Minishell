/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:51:27 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/08/28 11:50:39 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H
# include "./minishell.h"

void	handle_exec(t_data *data);
char	*get_path(char *cmd, t_data *data);
t_list	*add_cmd(char *cmd, t_data *data);
void	pipex(t_data *data);
void	make_dups_pipe(t_redir data);
void	exec(t_parse *lst, t_data *data);

#endif