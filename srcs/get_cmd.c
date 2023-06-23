/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 17:53:48 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/06/24 00:17:32 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_and_exit(t_data *data)
{
	free_all(&data->collector);
	exit(1);
}

void	exec(t_parse *lst, t_data *data)
{
	execve(lst->path, lst->args, data->envp);
	ft_putstr_fd("exec failed\n", 2);
	free_and_exit(data);
}

int	get_cmd(t_parse *lst, t_data *data)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid)
		return (0);
	else
		exec(lst, data);
	return (SUCCESS);
}
