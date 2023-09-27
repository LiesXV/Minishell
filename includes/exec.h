/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:51:27 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/27 11:50:34 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H
# include "./minishell.h"

void	handle_exec(t_data *data);
char	*get_path(char *cmd, t_data *data);
t_list	*add_cmd(char *cmd, t_data *data);
void	pipex(t_data *data);
void	make_dups_pipe(t_redir redir, t_data *data);
void	exec(t_parse *lst, t_data *data);
void	ft_close(int *fd);
void	closefds_pipe(t_data *data);
void	free_and_exit(t_data *data);
void	wait_pids(int pid, int status, int is_last);
void	handle_hd(t_data *data, t_parse *cur);
void	make_dups(t_data *data);
void	handle_hd_pipes(t_data *data, t_piplist *cur);
void	switch_and_close_fds(t_data *data);
void	exec_pipe(t_piplist *lst, t_data *data);
int		read_end(int stdin_cpy, char *line);

#endif