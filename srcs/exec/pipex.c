/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 21:46:28 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/08/31 15:59:43 by ibenhaim         ###   ########.fr       */
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
	cur->redir.in = ft_strdup(file);
	return (fd);
}

int	read_input_pipes(t_piplist *cur)
{
	int		file;
	char	*line;

	file = open_a_tmp_pipes(cur);
	if (file < 0)
		return (-1);
	// ft_putstr(cur->redir.hd);
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(0);
		if (!line)
			exit(1);
		line[ft_strlen(line) - 1] = 0;
		if (!ft_strncmp(cur->redir.hd, line, ft_strlen(cur->redir.hd) + 1))
			break ;
		// ft_putstr(line);
		write(file, line, ft_strlen(line));
		write(file, "\n", 1);
		free(line);
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
		ft_close(cur->redir.sstdin);
		exec_pipe(cur, data);
		exit(1);
	}
	ft_close(cur->redir.sstdin);
	switch_and_close_fds(data);
	if (access(cur->redir.in, F_OK) == 0)
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
		cur = cur->next;
	}
	while (cpy)
	{
		wait(NULL);
		cpy = cpy->next;
	}
}
