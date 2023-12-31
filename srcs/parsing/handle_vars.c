/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 16:28:39 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/27 16:38:22 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	parse_add_back(t_parse **lst, t_parse *new)
{
	t_parse	*cur;

	if (lst && *lst)
	{
		cur = *lst;
		while (cur->next)
			cur = cur->next;
		cur->next = new;
	}
	else if (lst)
		*lst = new;
}

int	var_redir_undef(t_parse *elem, int space)
{
	elem->var_val = get_env_val(elem->p_data, elem->var);
	if (space == 1 && elem->var_val)
		elem->var_val = \
		handle_var_spaces(elem->var_val, elem->fullcmd, elem->i);
	if (add_address(&elem->p_data->collector, elem->var_val))
		return (-1);
	if (elem->var_val && space == 1 && elem->fullcmd[elem->i] != '$'
		&& (only_spaces(elem->var_val) || mid_space(elem->var_val)))
		error_undef(elem->var);
	if (!elem->var_val && space == 1 && elem->fullcmd[elem->i] != '$')
		error_undef(elem->var);
	free(elem->var);
	return (0);
}

int	find_var(t_parse *elem, int state)
{
	int	count;

	count = -1;
	if (state == 0)
		elem->i++;
	if (state == 0 && (elem->fullcmd[elem->i] == '\'' \
		|| elem->fullcmd[elem->i] == '"'))
	{
		elem->i--;
		return (FAILURE);
	}
	while (elem->fullcmd[elem->i] \
			&& !contains(elem->fullcmd[elem->i], "\\ $[]\",'/=|@"))
	{
		if (state == 0 && (elem->fullcmd[elem->i] == '\'' \
			|| elem->fullcmd[elem->i] == '"'))
			return (FAILURE);
		elem->var[++count] = elem->fullcmd[elem->i];
		elem->i++;
	}
	elem->var[count + 1] = 0;
	return (SUCCESS);
}

int	var_redir(t_parse *elem, int dir, int space)
{
	int	count;

	if (init_var_str(elem) == FAILURE)
		return (-1);
	elem->var[0] = 0;
	elem->i++;
	if (elem->fullcmd[elem->i] == '\'' || elem->fullcmd[elem->i] == '"')
		return (0);
	if (var_state(elem, dir + 2, 0) == 1)
		return (1);
	find_var(elem, 1);
	if (var_redir_undef(elem, space) == -1)
		return (-1);
	count = test_value(elem);
	if (count != 0)
	{
		if (elem->var_val)
			elem->var_val = NULL;
		return (count);
	}
	place_var(elem, dir + 2, 0);
	return (elem->i--, -1);
}

int	var_handler(t_parse *elem, int isarg, int nb, int keep_space)
{
	if (dollar_error(elem, isarg, nb) != SUCCESS \
		|| var_state(elem, isarg, nb) == 1)
		return (1);
	if (init_var_str(elem) == FAILURE)
		return (-1);
	if (find_var(elem, 0) != SUCCESS)
		return (0);
	elem->var_val = get_env_val(elem->p_data, elem->var);
	if (keep_space == 1 && elem->var_val)
	{
		elem->var_val = \
		handle_var_spaces(elem->var_val, elem->fullcmd, elem->i);
		if (add_address(&elem->p_data->collector, elem->var_val))
			return (-1);
	}
	keep_space = test_value(elem);
	if (keep_space != 0)
	{
		if (elem->var_val)
			elem->var_val = NULL;
		return (keep_space);
	}
	place_var(elem, isarg, nb);
	return (elem->var_val = NULL, elem->i--, 1);
}
