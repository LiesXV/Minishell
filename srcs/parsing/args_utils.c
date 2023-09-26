/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 19:48:11 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/23 19:45:28 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	free_tmp(t_parse *elem, int i)
{
	while (i)
	{
		elem->args[i - 1] = ft_strdup(elem->tmp[i - 1]);
		if (!elem->args[i - 1]
			|| add_address(&elem->p_data->collector, elem->args[i - 1]))
			return (FAILURE);
		i--;
	}
	return (SUCCESS);
}

int	arg_is_blank(char *arg, t_parse *elem)
{
	int	i;

	i = elem->i - 1;
	if (!arg[0] && i >= 2 && (elem->fullcmd[elem->i - 1] == '"'
			|| elem->fullcmd[elem->i - 1] == '\'')
		&& (elem->fullcmd[elem->i - 2] == '"'
			|| elem->fullcmd[elem->i - 2] == '\'') && !elem->fullcmd[elem->i])
	{
		while (i >= 2 && (elem->fullcmd[i] == '"' || elem->fullcmd[i] == '\''))
		{
			if (elem->fullcmd[i - 1] != elem->fullcmd[i])
				return (0);
			i = i - 2;
		}
		if (!contains(elem->fullcmd[elem->i], " \t\n\r\v\f"))
			return (0);
		return (1);
	}
	return (0);
}

int	init_parse_arg(t_parse *elem, int nb)
{
	if (nb < 0)
		return (FAILURE);
	elem->j = -1;
	elem->args[nb] = NULL;
	elem->args[nb] = \
	(char *)malloc(sizeof(char) * (ft_strlen(elem->fullcmd) + 1));
	if (!elem->args[nb]
		|| add_address(&elem->p_data->collector, elem->args[nb]))
		return (FAILURE);
	elem->args[nb][0] = 0;
	while (elem->fullcmd[elem->i]
		&& contains(elem->fullcmd[elem->i], " \t\n\r\v\f"))
		elem->i++;
	elem->i--;
	return (SUCCESS);
}

int	init_args(t_parse *elem, int nb, int j)
{
	if (j == 1)
	{
		elem->arg = NULL;
		elem->tmp = NULL;
		elem->args[0] = ft_strdup(elem->cmd);
		if (!elem->args[0]
			|| add_address(&elem->p_data->collector, elem->args[0]))
			return (FAILURE);
	}
	if (j == 2)
	{
		elem->tmp = elem->args;
		elem->args = (char **)malloc(sizeof(char *) * (nb + 2));
		if (!elem->args || add_address(&elem->p_data->collector, elem->args))
			return (FAILURE);
	}
	return (SUCCESS);
}

int	arg_redir(t_parse *elem, int err, int nb)
{
	if (elem->fullcmd[elem->i]
		&& elem->fullcmd[elem->i] == '$'
		&& elem->fullcmd[elem->i - 1] != '\\')
	{
		if (elem->fullcmd[elem->i + 1] == '\\')
			elem->args[nb][++elem->j] = elem->fullcmd[elem->i];
		else
			err = var_handler(elem, 1, nb, 1);
	}
	if ((elem->fullcmd[elem->i] == '>'
			|| elem->fullcmd[elem->i] == '<')
		&& elem->fullcmd[elem->i - 1] != '\\')
		err = redir(elem, 0);
	return (err);
}
