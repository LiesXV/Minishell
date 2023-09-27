/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 23:37:18 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/27 16:41:09 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	in_child_process(pid_t pid, t_data *data, t_parse *cur)
{
	int	status;

	status = 0;
	if (pid == 0)
	{
		make_dups(data);
		cur->path = get_path(cur->cmd, data);
		if (add_address(&data->collector, cur->path) == 1)
			return ;
		if (!only_spaces(cur->cmd))
			exec(cur, data);
	}
	wait_pids(pid, status, 0);
	ft_close(&data->infile);
	if (cur->redir.hd && cur->redir.in && access(cur->redir.in, F_OK) == 0)
		unlink(cur->redir.in);
}

void	handle_exec(t_data *data)
{
	pid_t	pid;
	t_parse	*cur;

	cur = *data->cmd_lst;
	data->infile = cur->redir.sstdin;
	data->outfile = cur->redir.sstdout;
	if (cur->piplist)
		pipex(data);
	else
	{
		if (cur->redir.hd)
			handle_hd(data, cur);
		if (cur->args[0][0] && \
			(is_builtin(cur->args, data) == FAILURE))
		{
			pid = fork();
			if (pid < 0)
				return ;
			in_child_process(pid, data, cur);
		}
		else if (!cur->args[0][0])
			g_end_status = 0;
	}
	ft_close(&data->outfile);
}
