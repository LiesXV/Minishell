/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 21:46:28 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/08/30 12:41:41 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_close(int fd)
{
	if (fd > 2)
		close(fd);
}

void	switch_and_close_fds(t_data *data)
{
	ft_close(data->new_fd[1]);
	ft_close(data->old_fd[0]);
	data->old_fd[0] = data->new_fd[0];
	data->old_fd[1] = data->new_fd[1];
	return ;
}

void	make_dups_pipe(t_redir redir, t_data *data)
{
	if (redir.sstdin > 2)
	{
		if (dup2(redir.sstdin, STDIN_FILENO) == -1)
			exit(1);
		ft_close(redir.sstdin);
	}
	else
	{
		if (dup2(data->old_fd[0], STDIN_FILENO) == -1)
			exit(1);
		ft_close(data->old_fd[0]);
		ft_close(data->new_fd[0]);
	}
	if (redir.sstdout > 2)
	{
		if (dup2(redir.sstdout, STDOUT_FILENO) == -1)
			exit(1);
		ft_close(redir.sstdout);
		ft_close(data->new_fd[1]);
	}
	else
	{
		if (dup2(data->new_fd[1], STDOUT_FILENO) == -1)
			exit(1);
		ft_close(data->new_fd[1]);
	}
	ft_close(data->new_fd[0]);
}
void	exec_pipe(t_piplist *lst, t_data *data)
{
	if ((is_builtin(lst->cmd, data) == FAILURE))
	{
		execve(lst->path, lst->cmd, data->envp);
		ft_putstr_fd("minishellooooo: ", (*data->cmd_lst)->redir.sstdout);
		ft_putstr_fd(lst->cmd[0], (*data->cmd_lst)->redir.sstdout);
		ft_putstr_fd(": command not found\n", (*data->cmd_lst)->redir.sstdout);
		g_end_status = 127;
		// free_and_exit(data);
	}
	exit(1);
}

void	redir_pipes(t_data *data, t_piplist *cur)
{
	int	pid;

	if (cur->next)
	{
		if (pipe(data->new_fd) == FAILURE)
			return ;
	}
	else
		data->new_fd[1] = STDOUT_FILENO;
	pid = fork();
	if (pid < 0)
		return ;
	if (pid == 0)
	{
		make_dups_pipe(cur->redir, data);
		ft_close(cur->redir.sstdin);
		exec_pipe(cur, data);
		exit(1);
	}
	ft_close(cur->redir.sstdin);
	switch_and_close_fds(data);
}

void	pipex(t_data *data)
{
	// int			pid1;
	// int			pid2;
	t_piplist	*cpy;
	t_piplist	*cur;
	t_parse		*cmd_lst;

	cmd_lst = *(data->cmd_lst);
	cur = *cmd_lst->piplist;
	cpy = cur;
	data->old_fd[0] = 0;
	data->old_fd[1] = 1;
	while (cur)
	{
		redir_pipes(data, cur);
		cur = cur->next;
	}
	while (cpy)
	{
		wait(NULL);
		cpy = cpy->next;
	}
}
