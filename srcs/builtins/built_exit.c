/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 17:39:38 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/08/22 22:03:52 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_alphanumeric(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] > '9' || str[i] < '0')
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}

void	built_exit(t_data *data, char **args)
{
	int	i;

	i = 0;
	ft_putstr_fd("coco\n", 2);
	while (args[i])
	{
		if (!ft_strncmp("exit", args[i], 4))
			break ;
		i++;
	}
	ft_putstr_fd("exit\n", (*data->cmd_lst)->redir.sstderr);
	if (args[i + 1] && is_alphanumeric(args[i + 1]) == FAILURE)
		printf("minishell: exit: %s: numeric argument required\n", args[i + 1]);
	else if (args[i + 1])
		exit(ft_atoi(args[1]));
	else
		exit(g_end_status);
	free_all(&data->collector);
}
