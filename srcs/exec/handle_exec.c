/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 23:37:18 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/07 16:46:56 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	make_dups(t_data *data)
{
	if (data->infile > 2)
	{
		if (dup2(data->infile, STDIN_FILENO) == -1)
		{
			free_and_exit(data);
			exit(g_end_status);
		}
		close(data->infile);
	}
	if (data->outfile > 2)
	{
		if (dup2(data->outfile, STDOUT_FILENO) == -1)
		{
			free_and_exit(data);
			exit(g_end_status);
		}
		close(data->outfile);
	}
}

void	free_and_exit(t_data *data)
{
	free_all_env(&data->env);
	free_all(&data->collector);
	printf("val2 : %d\n", g_end_status);
	exit(g_end_status);
}

void	exec(t_parse *lst, t_data *data)
{
	execve(lst->path, lst->args, data->envp);
	ft_putstr_fd("minishell: ", (*data->cmd_lst)->redir.sstdout);
	ft_putstr_fd(lst->args[0], (*data->cmd_lst)->redir.sstdout);
	ft_putstr_fd(": command not found\n", (*data->cmd_lst)->redir.sstdout);
	g_end_status = 127;
	// printf("val2 : %d\n", g_end_status);
	free_and_exit(data);
}

int	open_a_tmp(t_parse *cur)
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
	cur->redir.in = ft_strdup(file);
	if (!cur->redir.in)
		return (free(file), -1);
	fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	free(file);
	if (fd == -1)
		return (-1);
	return (fd);
}

int	read_input(t_data *data, t_parse *cur)
{
	int		file;
	char	*line;
	int		i;

	i = 0;
	line = NULL;
	file = open_a_tmp(cur);
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
		if (cur->redir.hd[i + 1])
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
	data->infile = open(cur->redir.in, O_RDONLY);
	return (0);
}

void	handle_hd(t_data *data, t_parse *cur)
{
	if (read_input(data, cur) == -1)
		printf("error creating here doc\n");
	return ;
}

void    handle_exec(t_data *data)
{
	pid_t	pid;
	t_parse	*cur;
	int		status;

	status = 0;
	cur = *data->cmd_lst;
	data->infile = cur->redir.sstdin;
	data->outfile = cur->redir.sstdout;
	if (cur->piplist)
		pipex(data);
	else if ((is_builtin(cur->args, data) == FAILURE))
	{
		if (cur->redir.hd)
			handle_hd(data, cur);
		pid = fork();
		if (pid < 0)
			return ;
		if (pid == 0)
		{
			make_dups(data);
			cur->path = get_path(cur->cmd, data);
			if (add_address(&data->collector, cur->path) == 1)
				return ;
			if (cur->cmd && !only_spaces(cur->cmd))
				exec(cur, data);
		}
		if (waitpid(pid, &status, 0) == FAILURE)
			g_end_status = 1;
		else if (WIFEXITED(status))
			g_end_status = WEXITSTATUS(status);
		printf("val1 : %d\n", g_end_status);
		if (cur->redir.hd && cur->redir.in && access(cur->redir.in, F_OK) == 0)
			unlink(cur->redir.in);
	}
	ft_close(&data->outfile);
	ft_close(&data->infile);
}
