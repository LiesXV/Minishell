/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_vars_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 19:29:52 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/24 22:49:15 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	error_undef(char *name)
{
	ft_putstr_fd("minishell: $", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd(": undefined redirect\n", 2);
	g_end_status = 1;
}

int	dollar_error(t_parse *elem, int isarg, int nb)
{
	int	err;

	err = 0;
	while (elem->fullcmd[elem->i] && elem->fullcmd[elem->i] == '$' 
		&& (elem->fullcmd[elem->i + 1] == 0 || elem->fullcmd[elem->i + 1] == '$'
			|| (elem->i > 0 && elem->fullcmd[elem->i - 1] == '$') 
			|| contains(elem->fullcmd[elem->i + 1], " \t\n\r\v\f")))
	{
		if (isarg == 0)
			elem->cmd[++elem->j] = elem->fullcmd[elem->i];
		if (isarg == 1)
			elem->args[nb][++elem->j] = elem->fullcmd[elem->i];
		elem->i++;
		if (ft_strlen(elem->fullcmd) == 1 && elem->fullcmd[0] == '$')
			return (1);
		err++;
	}
	if (err > 0)
	{
		elem->i--;
		return (1);
	}
	return (0);
}

int	init_var_str(t_parse *elem)
{
	elem->var = NULL;
	elem->var_val = NULL;
	elem->var = malloc(sizeof(char) * (ft_strlen(elem->fullcmd) + 1));
	if (!elem->var || add_address(&elem->p_data->collector, elem->var) == 1)
		return (FAILURE);
	return (SUCCESS);
}

int	var_state(t_parse *elem, int isarg, int i)
{
	int		j;

	j = elem->i;
	if (isarg >= 2 && j != (int)ft_strlen(elem->fullcmd))
		j += 1;
	if (elem->fullcmd[j] && (j < (int)ft_strlen(elem->fullcmd)) \
		&& elem->fullcmd[j] == '$' && elem->fullcmd[j + 1] == '?')
	{
		if (j == 0 || (j > 0 && elem->fullcmd[j - 1] != '\\' \
			&& elem->fullcmd[j - 1] != '$'))
		{
			elem->var_val = ft_itoa(g_end_status);
			if (!elem->var_val || add_address(&elem->p_data->collector, elem->var_val))
				return (-1);
			place_var(elem, isarg, i);
			if (elem->var_val)
				elem->var_val = NULL;
			elem->i++;
			return (1);
		}
	}
	return (0);
}

int	test_value(t_parse *elem)
{
	if (!elem->var_val)
	{
		if (contains(elem->fullcmd[elem->i], "\\ $[]\",'/=|@"))
			elem->i--;
		if (elem->fullcmd[elem->i] == ' ' \
			&& elem->fullcmd[elem->i - 1] != '\\')
			return (-2);
		if (elem->fullcmd[elem->i] == '$' \
			&& elem->fullcmd[elem->i - 1] != '\\')
			elem->i--;
		return (1);
	}
	return (0);
}
