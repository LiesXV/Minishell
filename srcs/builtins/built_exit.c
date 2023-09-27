/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 17:39:38 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/27 17:05:21 by ibenhaim         ###   ########.fr       */
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

int	exit_end(char **args, int i)
{
	if (args[1] && ft_is_numeric(args[1]) == FAILURE)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[i], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		g_end_status = 2;
		return (FAILURE);
	}
	if (i >= 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		g_end_status = 1;
		return (SUCCESS);
	}
	return (FAILURE);
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
	if (exit_end(args, i) == SUCCESS)
		return (SUCCESS);
	if (i == 1 && args[i + j] && ft_is_numeric(args[i]) == SUCCESS)
		arg = ft_atoi(args[i + j]);
	free_all_env(&data->env);
	free_all(&data->collector);
	if (arg >= 0)
		exit(arg);
	else if (arg < 0 && i > 0)
		exit(156);
	return (exit(g_end_status), SUCCESS);
}
