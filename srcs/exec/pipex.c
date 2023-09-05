/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 21:46:28 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/05 14:06:28 by ibenhaim         ###   ########.fr       */
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
	if ((is_builtin(lst->cmd, data) == FAILURE))
	{
		execve(lst->path, lst->cmd, data->envp);
		ft_putstr_fd("minishell: ", (*data->cmd_lst)->redir.sstdout);
		ft_putstr_fd(lst->cmd[0], (*data->cmd_lst)->redir.sstdout);
		ft_putstr_fd(": command not found\n", (*data->cmd_lst)->redir.sstdout);
		g_end_status = 127;
		// free_and_exit(data);
	}
	exit(g_end_status);
}

int	open_a_tmp_pipes(t_piplist *cur)
{
	char	*file;
	int		fd;
	int		i;

	i = 1;
	file = ft_strdup(".heredoc_tmp");
	if (!file)
		return (-1);
	while (access(file, F_OK) == 0 && i < 256)
	{
		free(file);
		file = ft_strjoin(".heredoc_tmp", ft_itoa(i));
		i++;
	}
	if (i > 255)
		return (-1);
	fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	cur->redir.in = ft_strdup(file);
	return (fd);
}

int	read_input_pipes(t_piplist *cur)
{
	int		file;
	char	*line;
	int		i;

	i = 0;
	line = NULL;
	file = open_a_tmp_pipes(cur);
	if (file < 0)
		return (-1);
	while (cur->redir.hd[i])
	{
		while (1)
		{
			write(1, "> ", 2);
			line = get_next_line(0);
			if (!line)
				exit(g_end_status);
			line[ft_strlen(line) - 1] = 0;
			if (!ft_strncmp(cur->redir.hd[i], line, ft_strlen(cur->redir.hd[i]) + 1))
				break ;
			write(file, line, ft_strlen(line));
			write(file, "\n", 1);
			free(line);
		}
		if (cur->redir.hd[i])
		{
			close(file);
			file = open(cur->redir.in, O_WRONLY, O_TRUNC);
		}
		else
			break ;
		i++;
	}
	free(line);
	close(file);
	cur->redir.sstdin = open(cur->redir.in, O_RDONLY);
	return (0);
}

void	handle_hd_pipes(t_piplist *cur)
{
	while (cur)
	{
		if (cur->redir.hd)
		{
			if (read_input_pipes(cur) == -1)
				printf("error creating here doc\n");
		}
		cur = cur->next;
	}
	return ;
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
		closefds_pipe(data);
		ft_close(&cur->redir.sstdin);
		exec_pipe(cur, data);
		exit(g_end_status);
	}
	ft_close(&cur->redir.sstdin);
	switch_and_close_fds(data);
	if (cur->redir.hd && cur->redir.in && access(cur->redir.in, F_OK) == 0)
		unlink(cur->redir.in);
}

void	pipex(t_data *data)
{
	t_piplist	*cpy;
	t_piplist	*cur;
	t_parse		*cmd_lst;

	cmd_lst = *(data->cmd_lst);
	cur = *cmd_lst->piplist;
	cpy = cur;
	data->old_fd[0] = 0;
	data->old_fd[1] = 1;
	handle_hd_pipes(cur);
	while (cur)
	{
		redir_pipes(data, cur);
		ft_close(&cur->redir.sstdin);
		ft_close(&cur->redir.sstdout);
		cur = cur->next;
	}
	while (cpy)
	{
		wait(NULL);
		ft_close(&cpy->redir.sstdin);
		ft_close(&cpy->redir.sstdout);
		cpy = cpy->next;
	}
}
