/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 17:53:48 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/07/26 02:03:11 by lmorel           ###   ########.fr       */
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
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(lst->args[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	free_and_exit(data);
}

int	get_cmd(t_parse *lst, t_data *data)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid)
		return (SUCCESS);
	else
		exec(lst, data);
	return (SUCCESS);
}
