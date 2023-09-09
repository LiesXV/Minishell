/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 17:39:38 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/08 19:23:58 by ibenhaim         ###   ########.fr       */
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
	if (i == 1 && ft_is_numeric(args[i]) == FAILURE)
	{
		printf("minishell: exit: %s: numeric argument required\n", args[i]);
		g_end_status = 2;
	}
	if (i >= 2)
		return (ft_putstr_fd("minishell: exit: too many arguments\n", 2), 0);
	if (i == 1 && args[i + j] && ft_is_numeric(args[i]) == SUCCESS)
		arg = ft_atoi(args[i + j]);
	free_all_env(&data->env);
	free_all(&data->collector);
	if (arg >= 0)
		exit(arg);
	return (exit(g_end_status), SUCCESS);
}
