/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 04:20:49 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/25 13:33:47 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	redir_in(t_parse *elem, int i)
{
	redir_reset(elem, 0);
	elem->redir.in = calloc(sizeof(char), ft_strlen(elem->fullcmd) + 1);
	if (!elem->redir.in
		|| add_address(&elem->p_data->collector, elem->redir.in) == 1)
		return (-1);
	while (elem->fullcmd[elem->i]
		&& contains(elem->fullcmd[elem->i], " \t\n\r\v\f"))
		elem->i++;
	while (elem->fullcmd[elem->i]
		&& !contains(elem->fullcmd[elem->i], " \t\n\r\v\f|"))
	{
		i = redir_quote(elem, 0, elem->redir.in);
		if (i != 0 && i != 127 && elem->i != (int)ft_strlen(elem->fullcmd))
			return (i);
		if (redir_utils(elem, elem->redir.in, i) != 0)
			break ;
		elem->i++;
	}
	if (i != 127)
		elem->redir.in[elem->redir.i + 1] = 0;
	if (file_create(elem, 0) == -1)
		return (-1);
	return (1);
}

int	redir_out(t_parse *elem, int i)
{
	redir_reset(elem, 1);
	elem->redir.out1 = malloc(sizeof(char) * ft_strlen(elem->fullcmd) + 1);
	if (!elem->redir.out1
		|| add_address(&elem->p_data->collector, elem->redir.out1) == 1)
		return (-1);
	while (elem->fullcmd[elem->i]
		&& contains(elem->fullcmd[elem->i], " \t\n\r\v\f"))
		elem->i++;
	while (elem->fullcmd[elem->i]
		&& !contains(elem->fullcmd[elem->i], " \t\n\r\v\f|"))
	{
		i = redir_quote(elem, 1, elem->redir.out1);
		if (i != 0 && i != 127 && elem->i != (int)ft_strlen(elem->fullcmd))
			return (i);
		if (redir_utils(elem, elem->redir.out1, i) != 0)
			break ;
		elem->i++;
	}
	if (i != 127)
		elem->redir.out1[elem->redir.i + 1] = 0;
	if (file_create(elem, 1) == -1)
		return (-1);
	return (1);
}

int	redir_out_err(t_parse *elem, int i)
{
	redir_reset(elem, 2);
	elem->redir.out2 = malloc(sizeof(char) * ft_strlen(elem->fullcmd) + 1);
	if (!elem->redir.out2
		|| add_address(&elem->p_data->collector, elem->redir.out2) == 1)
		return (-1);
	while (elem->fullcmd[elem->i]
		&& contains(elem->fullcmd[elem->i], " \t\n\r\v\f"))
		elem->i++;
	while (elem->fullcmd[elem->i]
		&& !contains(elem->fullcmd[elem->i], " \t\n\r\v\f|"))
	{
		i = redir_quote(elem, 2, elem->redir.out2);
		if (i != 0 && i != 127 && elem->i != (int)ft_strlen(elem->fullcmd))
			return (i);
		if (redir_utils(elem, elem->redir.out2, i) != 0)
			break ;
		elem->i++;
	}
	if (i != 127)
		elem->redir.out2[elem->redir.i + 1] = 0;
	if (file_create(elem, 2) == -1)
		return (-1);
	return (1);
}

int	redir_util(t_parse *elem)
{
	if ((elem->redir.end == 0 && (elem->i == 2
				|| (elem->i > 3 && elem->fullcmd[elem->i - 2] == '2'
					&& contains(elem->fullcmd[elem->i - 3], " \t\n\r\v\f"))))
		|| (elem->redir.end == 1 && (elem->i == 3 || (elem->i > 4
					&& elem->fullcmd[elem->i - 3] == '2'
					&& (contains(elem->fullcmd[elem->i - 4], " \t\n\r\v\f"))))))
		return (redir_out_err(elem, 0));
	else
		return (redir_out(elem, 0));
}

int	redir(t_parse *elem, int i)
{
	elem->redir.i = -1;
	if (elem->fullcmd[elem->i + 2] && elem->fullcmd[elem->i] == '<'
		&& elem->fullcmd[elem->i + 1] == '<'
		&& elem->fullcmd[elem->i + 2] != '<')
		i = here_doc(elem);
	else if (elem->fullcmd[elem->i] == '>')
	{
		elem->redir.end = 0;
		elem->i++;
		if (elem->fullcmd[elem->i] == '>')
		{
			elem->redir.end = 1;
			elem->i++;
		}
		i = redir_util(elem);
	}
	else if (elem->fullcmd[elem->i] == '<')
		i = redir_in(elem, 0);
	if (elem->i >= (int)ft_strlen(elem->fullcmd))
		return (-3);
	return (i);
}
