/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 23:37:18 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/08/04 06:51:36 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_and_exit(t_data *data)
{
	free_all(&data->collector);
	exit(1);
}

void	exec(t_parse *lst, t_data *data)
{
	execve(lst->path, lst->args, data->envp);
	ft_putstr_fd("minishell: ", (*data->cmd_lst)->redir.sstdout);
	ft_putstr_fd(lst->args[0], (*data->cmd_lst)->redir.sstdout);
	ft_putstr_fd(": command not found\n", (*data->cmd_lst)->redir.sstdout);
	g_end_status = 127;
	free_and_exit(data);
}

void    handle_exec(t_data *data)
{
	t_parse	*cur;
	pid_t	pid;

	cur = *data->cmd_lst;
	if (is_builtin(cur->cmd, data) == FAILURE)
	{
		pid = fork();
		if (pid < 0)
			return ;
		if (pid == 0)
		{
			if (cur->piplist)
			{
				pipex(data);
				exit(1);
			}
			else
			{
				cur->path = get_path(cur->cmd, data);
				if (cur->cmd && !only_spaces(cur->cmd))
					exec(cur, data);
			}
		}
		wait(NULL);
	}
	return ;
}
