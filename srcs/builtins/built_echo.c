/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 12:17:25 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/06/24 12:17:25 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	built_echo(char **args)
{
	int	i;

	i = 1;
	if (!ft_strncmp(args[1], "-n", 2))
		i++;
	while (args[i])
	{
		if (ft_putstr(args[i]) != (int)ft_strlen(args[i]))
			return (ft_putstr_fd("write error", 2), FAILURE);
		ft_putchar(' ');
		i++;
	}
	if (ft_strncmp(args[1], "-n", 2))
		ft_putchar('\n');
	return (SUCCESS);
}
