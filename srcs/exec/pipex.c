/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 21:46:28 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/27 16:15:18 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	redir_pipes(t_data *data, t_piplist *cur)
{
	if (cur->next)
	{
		if (pipe(data->new_fd) == FAILURE)
			return ;
	}
	else
		data->new_fd[1] = STDOUT_FILENO;
	cur->pid = fork();
	if (cur->pid < 0)
		return ;
	if (cur->pid == 0)
	{
		if (cur->redir.sstdout < 0 || (!cur->redir.hd && cur->redir.sstdin < 0))
			free_and_exit(data);
		make_dups_pipe(cur->redir, data);
		closefds_pipe(data);
		ft_close(&cur->redir.sstdin);
		exec_pipe(cur, data);
		exit(g_end_status);
	}
	switch_and_close_fds(data, cur->redir.sstdin);
	if (cur->redir.in && cur->redir.hd && access(cur->redir.in, F_OK) == 0)
		unlink(cur->redir.in);
}

void	pipex_end(t_piplist *cur, t_piplist **cpy, t_data *data)
{
	int	status;

	status = 0;
	while (cur)
	{
		redir_pipes(data, cur);
		ft_close(&cur->redir.sstdin);
		ft_close(&cur->redir.sstdout);
		cur = cur->next;
	}
	while ((*cpy))
	{
		if ((*cpy)->next)
			wait_pids((*cpy)->pid, status, 0);
		else
			wait_pids((*cpy)->pid, status, 1);
		ft_close(&(*cpy)->redir.sstdin);
		ft_close(&(*cpy)->redir.sstdout);
		(*cpy) = (*cpy)->next;
	}
}

void	pipex(t_data *data)
{
	t_piplist	**cpy;
	t_piplist	*cur;
	t_parse		*cmd_lst;

	cmd_lst = *(data->cmd_lst);
	cur = *cmd_lst->piplist;
	cpy = &cur;
	data->old_fd[0] = 0;
	data->old_fd[1] = 1;
	handle_hd_pipes(data, cur);
	pipex_end(cur, cpy, data);
}
