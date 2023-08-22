/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 23:37:18 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/08/22 22:03:42 by lmorel           ###   ########.fr       */
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
	ft_putstr_fd("minishellooooo: ", (*data->cmd_lst)->redir.sstdout);
	ft_putstr_fd(lst->args[0], (*data->cmd_lst)->redir.sstdout);
	ft_putstr_fd(": command not found\n", (*data->cmd_lst)->redir.sstdout);
	g_end_status = 127;
	free_and_exit(data);
}

void    handle_exec(t_data *data)
{
	pid_t	pid;
	t_parse	*cur;

	cur = *data->cmd_lst;
	if (cur->piplist)
		pipex(data);
	else if ((is_builtin(cur->args, data) == FAILURE))
	{
		data->infile = cur->redir.sstdin;
		data->outfile = cur->redir.sstdout;
		pid = fork();
		if (pid < 0)
			return ;
		if (pid == 0)
		{
			make_dups(data);
			cur->path = get_path(cur->cmd, data);
			add_address(&data->collector, cur->path);
			if (cur->cmd && !only_spaces(cur->cmd))
				exec(cur, data);
			exit(1);
		}
		wait(NULL);
	}
}
