/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 19:48:11 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/23 19:44:55 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	arg_util(t_parse *elem, int nb)
{
	if (elem->fullcmd[elem->i] == '|' 
		&& elem->fullcmd[elem->i - 1] != '\\' && elem->j == -1)
	{
		elem->args[nb][++elem->j] = elem->fullcmd[elem->i];
		elem->args[nb][++elem->j] = 0;
		elem->i++;
		return (1);
	}
	if (elem->fullcmd[elem->i] == '|' && elem->fullcmd[elem->i - 1] != '\\')
		return (1);
	return (0);
}

int	copy_arg(t_parse *elem, int nb, int err)
{
	if (err == -3)
		return (1);
	if ((contains(elem->fullcmd[elem->i], " \t\n\r\v\f") 
			&& elem->fullcmd[elem->i - 1] != '\\') 
		&& (elem->args[nb][0] || (!elem->args[nb][0] 
		&& (elem->fullcmd[elem->i - 1] == '"' 
			|| elem->fullcmd[elem->i - 1] == '\'') 
		&& (elem->fullcmd[elem->i - 2] == '"' 
			|| elem->fullcmd[elem->i - 2] == '\'' || err == 1))))
		return (1);
	if (elem->fullcmd[elem->i] 
		&& (!contains(elem->fullcmd[elem->i], " \t\n\r\v\f") 
			|| (contains(elem->fullcmd[elem->i], " \t\n\r\v\f")
				&& elem->fullcmd[elem->i - 1] == '\\')) 
		&& err != 4 && err != 1 && ((elem->fullcmd[elem->i] == '$' 
				&& elem->fullcmd[elem->i - 1] != '$' 
				&& elem->fullcmd[elem->i + 1] == 0) 
			|| (elem->fullcmd[elem->i] == '$' 
				&& elem->fullcmd[elem->i - 1] == '\\') 
			|| (elem->fullcmd[elem->i] != '$')))
		elem->args[nb][++elem->j] = elem->fullcmd[elem->i];
	return (0);
}

char	*parse_arg(t_parse *elem, int nb)
{
	int	err;

	if (init_parse_arg(elem, nb) == FAILURE)
		return (NULL);
	while (elem->fullcmd[elem->i] && elem->fullcmd[++elem->i] 
		&& (!contains(elem->fullcmd[elem->i], " \t\n\r\v\f") 
			|| (contains(elem->fullcmd[elem->i], " \t\n\r\v\f") 
				&& elem->fullcmd[elem->i - 1] == '\\')))
	{
		if (arg_util(elem, nb) == 1)
			break ;
		err = 0;
		err = arg_quotes_handler(elem, nb, err);
		if (err == -1)
			return (NULL);
		err = arg_redir(elem, err, nb);
		if (copy_arg(elem, nb, err) == 1)
			break ;
	}
	elem->args[nb][++elem->j] = 0;
	return (elem->args[nb]);
}

int	form_args(t_parse *elem)
{
	int	i;

	init_args(elem, 0, 1);
	i = 1;
	while (1)
	{
		if (init_args(elem, i, 2) == FAILURE || free_tmp(elem, i) == FAILURE)
			return (FAILURE);
		//if (elem->tmp)
		//	free(elem->tmp);
		elem->arg = parse_arg(elem, i);
		if (elem->arg == NULL)
			return (FAILURE);
		if (arg_is_blank(elem->arg, elem))
			elem->arg = parse_arg(elem, ++i);
		if (!elem->arg || (!elem->arg[0] && !elem->fullcmd[elem->i]))
		{
		//	free(elem->arg);
			break ;
		}
		i++;
	}
	elem->args[i] = NULL;
	return (SUCCESS);
}
