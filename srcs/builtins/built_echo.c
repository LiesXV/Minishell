/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 19:24:21 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/08 20:08:57 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	built_echo_end(char	**args, int	*i)
{
	while (args[*i])
	{
		if (ft_putstr_fd(args[*i], STDOUT_FILENO) \
			!= (int)ft_strlen(args[*i]))
			return (ft_putstr_fd("write error", 2), FAILURE);
		if (args[*i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		*i = *i + 1;
	}
	return (SUCCESS);
}

int	built_echo(char **args)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (args[i] && !ft_strncmp(args[i], "echo", 4) \
		&& ft_strlen(args[i]) == 4)
		i++;
	if (!args[i])
		return (ft_putchar_fd('\n', STDOUT_FILENO), SUCCESS);
	if (!ft_strncmp(args[i], "-n", 2) && ft_strlen(args[i]) == 2)
	{
		j = i;
		i++;
		while (!ft_strncmp(args[i], "-n", 2))
			i++;
	}
	if (built_echo_end(args, &i) == FAILURE)
		return (FAILURE);
	if (!(!ft_strncmp(args[j], "-n", 2) && ft_strlen(args[1]) == 2))
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (SUCCESS);
}
