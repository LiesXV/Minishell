/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 16:02:45 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/07/27 02:15:06 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	built_cd(char **args)
{
	int	argc;

	argc = 0;
	while (args[argc])
		argc++;
	if (argc == 1)
	{
		if (chdir(getenv("HOME")) != 0)
		{
			perror("cd");
		}
	}
	else if (argc == 2)
	{
		if (chdir(args[1]) != 0)
		{
			perror("cd");
		}
	}
	else
	{
		ft_putstr_fd("cd: too much arguments\n", 2);
	}
	return (SUCCESS);
}
