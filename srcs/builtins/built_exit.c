/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 17:39:38 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/08/15 12:40:52 by ibenhaim         ###   ########.fr       */
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
	ft_putstr_fd("exit\n", (*data->cmd_lst)->redir.sstderr);
	if (args[1] && is_alphanumeric(args[1]) == FAILURE)
		printf("minishell: exit: %s: numeric argument required\n", args[1]);
	else if (args[1])
		exit(ft_atoi(args[1]));
	else
		exit(g_end_status);
	free_all(&data->collector);
}
