/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 16:02:45 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/06/24 16:02:45 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	built_cd(char **args)
{
	int	argc;

	argc = 0;
	while (args[argc])
		argc++;
	if (argc == 1)
	{
		ft_putstr_fd("cd: not enough arguments\n", 2);
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
