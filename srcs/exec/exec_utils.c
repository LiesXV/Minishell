/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 18:42:12 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/24 21:33:54 by lmorel           ###   ########.fr       */
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
	exit(g_end_status);
}

void	exec(t_parse *lst, t_data *data)
{
	if (lst->redir.sstdin < 0 || lst->redir.sstdout < 0)
		free_and_exit(data);
	if (lst->path && (is_builtin(lst->args, data, lst->redir) == FAILURE))
	{
		execve(lst->path, lst->args, data->envp);
		ft_putstr_fd("minishell: ", (*data->cmd_lst)->redir.sstderr);
		ft_putstr_fd(lst->args[0], (*data->cmd_lst)->redir.sstderr);
		ft_putstr_fd(": command not found\n", (*data->cmd_lst)->redir.sstderr);
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
