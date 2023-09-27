/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:04:34 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/27 11:35:59 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	built_pwd(char **args)
{
	char	cwd[1024];

	if (args[1] && !ft_strcmp("-", args[1]))
	{
		ft_putstr_fd("minishell: pwd: does not support any options\n", \
			STDOUT_FILENO);
		g_end_status = 2;
		return (SUCCESS);
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_putstr_fd(cwd, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
	else
	{
		ft_putstr_fd("minishell: pwd: error retrieving current directory\n", 2);
		g_end_status = 0;
		return (FAILURE);
	}
	return (SUCCESS);
}
