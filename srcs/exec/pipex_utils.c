/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 19:07:29 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/27 15:51:57 by ibenhaim         ###   ########.fr       */
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

void	switch_and_close_fds(t_data *data, int fd)
{
	ft_close(&data->old_fd[1]);
	ft_close(&data->old_fd[0]);
	data->old_fd[0] = data->new_fd[0];
	data->old_fd[1] = data->new_fd[1];
	ft_close(&fd);
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
	int	i;

	i = 0;
	if (lst->cmd && is_builtin(lst->cmd, data) == SUCCESS)
		free_and_exit(data);
	if (lst->path != NULL && \
		is_executable(lst->cmd[0], lst->path) == FAILURE)
		free_and_exit(data);
	g_end_status = 0;
	execve(lst->path, lst->cmd, data->envp);
	print_error_msg(": command not found\n", 2, lst->cmd[0]);
	g_end_status = 127;
	free_and_exit(data);
}
