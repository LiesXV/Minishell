/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 21:46:28 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/08/21 15:33:04 by ibenhaim         ###   ########.fr       */
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
void	exec_pipe(t_piplist *lst, t_data *data)
{
	if ((is_builtin(lst->cmd[0], data) == FAILURE))
	{
		execve(lst->path, lst->cmd, data->envp);
		ft_putstr_fd("minishellooooo: ", (*data->cmd_lst)->redir.sstdout);
		ft_putstr_fd(lst->cmd[0], (*data->cmd_lst)->redir.sstdout);
		ft_putstr_fd(": command not found\n", (*data->cmd_lst)->redir.sstdout);
		g_end_status = 127;
		// free_and_exit(data);
		exit(1);
	}
}

void	redir_pipes(t_data *data, t_piplist *cur)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
		exit(1);
	if (!ft_strncmp("exit", cur->cmd[0], 4) && ft_strlen(cur->cmd[0]) == 4)
		built_exit(data, cur->cmd);
	else if (!ft_strncmp("cd", cur->cmd[0], 2) && ft_strlen(cur->cmd[0]) == 2)
		built_cd(cur->cmd);
	else
	{
		pid = fork();
		if (pid == -1)
			exit(1);
		if (pid)
		{
			close(pipefd[1]);
			if (dup2(pipefd[0], STDIN_FILENO) == -1)
				exit(1);
			close(pipefd[0]);
			ft_putstr_fd(cur->cmd[0], 2);
		}
		else
		{
			close(pipefd[0]);
			if (dup2(pipefd[1], STDOUT_FILENO) == -1)
				exit(1);
			close(pipefd[1]);
			exec_pipe(cur, data);
		}
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
	while (cur->next)
	{
		ft_putstr_fd("coco\n", 2);
		redir_pipes(data, cur);
		cur = cur->next;
	}
	if (!ft_strncmp("exit", cur->cmd[0], 4) && ft_strlen(cur->cmd[0]) == 4)
		built_exit(data, cur->cmd);
	else if (!ft_strncmp("cd", cur->cmd[0], 2) && ft_strlen(cur->cmd[0]) == 2)
		built_cd(cur->cmd);
	else
	{
		pid1 = fork();
		if (pid1 < 0)
			exit(1);
		if (pid1 == 0)
			exec_pipe(cur, data);
	}
	while (cpy)
	{
		wait(NULL);
		cpy = cpy->next;
	}
	wait(NULL);
}
