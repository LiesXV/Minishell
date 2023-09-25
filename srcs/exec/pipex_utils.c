/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 19:07:29 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/25 14:19:57 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_close(int *fd)
{
	if (*fd > 2)
	{
		close(*fd);
		*fd = -1;
	}
}

void	ft_closeall(t_data *data, int *fd1, int *fd2)
{
	ft_close(&data->new_fd[0]);
	ft_close(&data->new_fd[1]);
	ft_close(&data->old_fd[0]);
	ft_close(&data->old_fd[1]);
	ft_close(fd1);
	ft_close(fd2);
}

void	switch_and_close_fds(t_data *data)
{
	ft_close(&data->old_fd[1]);
	ft_close(&data->old_fd[0]);
	data->old_fd[0] = data->new_fd[0];
	data->old_fd[1] = data->new_fd[1];
	return ;
}

void	make_dups_pipe(t_redir redir, t_data *data)
{
	if (redir.sstdin > 2)
	{
		if (dup2(redir.sstdin, STDIN_FILENO) == -1)
			exit(g_end_status);
	}
	else
	{
		if (dup2(data->old_fd[0], STDIN_FILENO) == -1)
			exit(g_end_status);
	}
	if (redir.sstdout > 2)
	{
		if (dup2(redir.sstdout, STDOUT_FILENO) == -1)
			exit(g_end_status);
	}
	else
	{
		if (dup2(data->new_fd[1], STDOUT_FILENO) == -1)
			exit(g_end_status);
	}
	ft_closeall(data, &redir.sstdin, &redir.sstdout);
}

void	exec_pipe(t_piplist *lst, t_data *data)
{
	if (lst->redir.sstdin < 0 || lst->redir.sstdout < 0)
		free_and_exit(data);
	if (lst->cmd[0] && (is_builtin(lst->cmd, data, lst->redir) == FAILURE))
	{
		execve(lst->path, lst->cmd, data->envp);
		ft_putstr_fd("minishell: ", (*data->cmd_lst)->redir.sstderr);
		ft_putstr_fd(lst->cmd[0], (*data->cmd_lst)->redir.sstderr);
		ft_putstr_fd(": command not found\n", (*data->cmd_lst)->redir.sstderr);
		g_end_status = 127;
	}
	free_and_exit(data);
}
