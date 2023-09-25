/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 18:42:12 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/25 17:11:34 by ibenhaim         ###   ########.fr       */
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
	}
	if (data->outfile > 2)
	{
		if (dup2(data->outfile, STDOUT_FILENO) == -1)
		{
			free_and_exit(data);
			exit(g_end_status);
		}
	}
	ft_close(&data->infile);
	ft_close(&data->outfile);
}

void	print_error_msg(char *msg, int fd, char *cmd)
{
	ft_putstr_fd("minishell: ", fd);
	ft_putstr_fd(cmd, fd);
	ft_putstr_fd(msg, fd);
}

int	is_executable(char *cmd)
{
	struct stat	info;
	char		*str;

	str = NULL;
	if (stat(cmd, &info) == 0)
	{
		if (!(info.st_mode & S_IXUSR))
		{
			g_end_status = 126;
			print_error_msg(": Permission denied\n", 2, cmd);
			return (FAILURE);
		}
		if (S_ISDIR(info.st_mode))
		{
			g_end_status = 126;
			print_error_msg(": Is a directory\n", 2, cmd);
			return (FAILURE);
		}
		return (SUCCESS);
	}
	print_error_msg(": No such file or directory\n", 2, cmd);
	g_end_status = 127;
	return (FAILURE);
}

void	exec(t_parse *lst, t_data *data)
{
	if (lst->redir.sstdin < 0 || lst->redir.sstdout < 0)
		free_and_exit(data);
	if (is_builtin(lst->args, data, lst->redir) == FAILURE)
	{
		g_end_status = 1;
		if (lst->path && is_executable(lst->args[0]) == SUCCESS)
			execve(lst->path, lst->args, data->envp);
		print_error_msg(": Command not found\n", 2, lst->args[0]);
		g_end_status = 127;
	}
	free_and_exit(data);
}

void	wait_pids(int pid, int status, int is_last)
{
	if (waitpid(pid, &status, 0) == FAILURE)
		g_end_status = 1;
	else if (WIFEXITED(status))
		g_end_status = WEXITSTATUS(status);
	if (WIFSIGNALED(status) && is_last == 0)
	{
		if (WTERMSIG(status) == SIGQUIT)
		{
			g_end_status = 131;
			ft_putstr_fd("Quit core dumped\n", 2);
		}
		else if (WTERMSIG(status) == SIGINT)
		{
			g_end_status = 130;
			ft_putstr_fd("\n", 2);
		}
	}
}
