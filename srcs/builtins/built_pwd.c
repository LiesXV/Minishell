/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:04:34 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/08/21 11:59:12 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	built_pwd(t_data *data)
{
	char	cwd[1024];
	t_parse	*cur;

	cur = *data->cmd_lst;
	if (cur->args[1] && !ft_strcmp("-", cur->args[1]) && !cur->piplist)
	{
		ft_putstr("minishell: pwd: does not support any options\n");
		return (SUCCESS);
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_putstr(cwd);
		ft_putstr("\n");
	}
	else
	{
		perror("getcwd() error");
		return (FAILURE);
	}

	return (SUCCESS);
}
