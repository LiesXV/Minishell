/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 21:46:28 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/08/17 10:51:26 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	make_dups(t_data *data)
{
	if (data->outfile > 2)
	{
		if (dup2(data->outfile, STDOUT_FILENO) == -1)
		{
			// free_all(args);
			exit(1);
		}
		close(data->outfile);
	}
	if (data->infile > 2)
	{
		if (dup2(data->infile, STDIN_FILENO) == -1)
		{
			// free_all(args);
			exit(1);
		}
		close(data->infile);
	}
}

void	redir_pipes(t_data *data, t_piplist *cur)
{
	int		pipefd[2];
	pid_t	pid;
	t_parse		*cmd_lst;

	cmd_lst = *(data->cmd_lst);
	if (pipe(pipefd) == -1)
		exit(1);
	pid = fork();
	if (pid == -1)
		exit(1);
	if (pid)
	{
		close(pipefd[1]);
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
			exit(1);
		close(pipefd[0]);
	}
	else
	{
		close(pipefd[0]);
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
			exit(1);
		close(pipefd[1]);
		execve(cur->path, cur->cmd, data->envp);
	}
}

void	pipex(t_data *data)
{
	int			pid1;
	t_piplist	*cpy;
	t_piplist	*cur;
	t_parse		*cmd_lst;

	cmd_lst = *(data->cmd_lst);
	cur = *cmd_lst->piplist;
	cpy = cur;
	data->infile = cmd_lst->redir.sstdin;
	data->outfile = cmd_lst->redir.sstdout;
	make_dups(data);
	while (cur->next)
	{
		redir_pipes(data, cur);
		cur = cur->next;
	}
	pid1 = fork();
	if (pid1 < 0)
		exit(1);
	if (pid1 == 0)
		execve(cur->path, cur->cmd, data->envp);
	while (cpy)
	{
		wait(NULL);
		cpy = cpy->next;
	}
	wait(NULL);
}
