/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 21:46:28 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/07/31 21:46:28 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_and_exit_pipe(void)
{
	printf("free et excite.\n");
	exit(1);
}

void	exec_pipe(t_piplist *lst, t_data *data)
{
	get_next_line(0);
	execve(lst->path, lst->cmd, data->envp);
	ft_putstr_fd("exec failed\n", 2);
	free_and_exit_pipe();

}

void	redir_pipe(t_piplist *lst, t_data *data)
{
	pid_t	pid;
	int		pipefd[2];

	if (pipe(pipefd) == -1)
		free_and_exit_pipe();

	pid = fork();
	if (pid == -1)
		free_and_exit_pipe();

	if (pid)
	{
		close(pipefd[1]);
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
			free_and_exit_pipe();

		close(pipefd[0]);
	}
	else
	{
		close(pipefd[0]);
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
			free_and_exit_pipe();

		close(pipefd[1]);
		exec_pipe(lst, data);
	}
}

void	make_dups(t_parse	*cmd_lst)
{
	if (cmd_lst->redir.sstdout != -1)
	{
		if (dup2(cmd_lst->redir.sstdout, STDOUT_FILENO) == -1)
		{
			printf("stdout == -1\n");
			get_next_line(0);
		}
		close(cmd_lst->redir.sstdout);
	}
	if (cmd_lst->redir.sstdin != -1)
	{
		if (dup2(cmd_lst->redir.sstdin, STDIN_FILENO) == -1)
		{
			printf("stdin == -1\n");
			get_next_line(0);
		}
		close(cmd_lst->redir.sstdin);
	}
}

void	pipex(t_data *data)
{
	int			pid1;
	t_parse		*cmd_lst;
	t_piplist	*cur;

	cmd_lst = *(data->cmd_lst);
	cur = *cmd_lst->piplist;
	// make_dups(cmd_lst);
	while (cur->next)
	{
		redir_pipe(cur, data);
		cur = cur->next;
	}
	pid1 = fork();
	if (pid1 == -1)
		free_and_exit_pipe();
	if (pid1 == 0)
		exec_pipe(cur, data);
	while (cmd_lst)
	{
		wait(NULL);
		cmd_lst = cmd_lst->next;
	}
	wait(NULL);
}
