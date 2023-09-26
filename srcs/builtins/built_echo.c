/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 19:24:21 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/26 13:06:29 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	built_echo_end(char	**args, int	*i)
{
	while (args[*i])
	{
		if (printf("%s", args[*i]) != (int)ft_strlen(args[*i]))
			return (ft_putstr_fd("write error", 2), FAILURE);
		if (args[*i + 1])
			printf(" ");
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
		return (g_end_status = 0, printf("\n"), SUCCESS);
	if (args[i] && !ft_strncmp(args[i], "-n", 2))
	{
		j = i;
		i++;
		while (args[i] && !ft_strncmp(args[i], "-n", 2))
			i++;
	}
	if (built_echo_end(args, &i) == FAILURE)
		return (g_end_status = 0, SUCCESS);
	if (!(!ft_strncmp(args[j], "-n", 2)))
		printf("\n");
	return (g_end_status = 0, SUCCESS);
}
