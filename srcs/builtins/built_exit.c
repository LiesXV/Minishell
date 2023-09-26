/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 17:39:38 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/26 10:58:32 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_valid(int c)
{
	if ((c >= '0' && c <= '9') || (c == ' ') || (c == '-' || c == '+'))
	{
		return (SUCCESS);
	}
	else
		return (FAILURE);
}

static int	ft_is_numeric(char *nb)
{
	int	i;

	i = 0;
	while (nb[i] != '\0')
	{
		if (is_valid(nb[i]) == FAILURE)
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}

int	nb_line(char **args)
{
	int	i;
	int	j;

	i = 0;
	while (args[i] && !ft_strncmp("exit", args[i], 4))
		i++;
	j = 0;
	while (args[i + j])
		j++;
	return (j);
}

void	exit_end(char **args, int i)
{
	if (i == 1 && ft_is_numeric(args[i]) == FAILURE)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[i], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		g_end_status = 2;
	}
	if (i >= 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		g_end_status = 1;
	}
}

int	built_exit(t_data *data, char **args)
{
	int	i;
	int	arg;
	int	j;

	arg = -1;
	i = nb_line(args);
	j = 0;
	while (args[j] && !ft_strncmp("exit", args[i], 4))
		j++;
	if (!(*data->cmd_lst)->piplist)
		ft_putstr_fd("exit\n", (*data->cmd_lst)->redir.sstderr);
	exit_end(args, i);
	if (i == 1 && args[i + j] && ft_is_numeric(args[i]) == SUCCESS)
		arg = ft_atoi(args[i + j]);
	free_all_env(&data->env);
	free_all(&data->collector);
	if (arg >= 0)
		exit(arg);
	return (exit(g_end_status), SUCCESS);
}
