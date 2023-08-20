/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:04:34 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/08/20 15:57:17 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	built_pwd(t_data *data)
{
	char	cwd[1024];
	t_parse	*cur;

	cur = *data->cmd_lst;

	if (cur->args[1] && !ft_strcmp("-", cur->args[1]))
	{
		ft_putstr_fd("minishell: pwd: does not support any options\n", 2);
		return (SUCCESS);
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_putstr_fd(cwd, (*data->cmd_lst)->redir.sstdout);
		ft_putstr_fd("\n", (*data->cmd_lst)->redir.sstdout);
	}
	else
	{
		perror("getcwd() error");
		return (FAILURE);
	}

	return (SUCCESS);
}
