/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 04:20:49 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/08 19:16:27 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	redir_reset(t_parse *elem, int i)
{
	if (i == 0)
		elem->i++;
	if ((i == 0 && elem->redir.in != NULL) 
		|| (i == 1 && elem->redir.out1 != NULL)
		|| (i == 2 && elem->redir.out2 != NULL) 
		|| (i == 15 && elem->redir.hd != NULL))
	{
		rlist_add_back(elem->rlist, new_rlist_elem(elem));
		elem->redir.in = NULL;
		elem->redir.sstdin = 0;
		elem->redir.out1 = NULL;
		elem->redir.sstdout = 1;
		elem->redir.out2 = NULL;
		elem->redir.sstderr = 2;
	}
}

int	redir_in(t_parse *elem, int i)
{
	redir_reset(elem, 0);
	elem->redir.in = malloc(sizeof(char) * ft_strlen(elem->fullcmd) + 1);
	if (!elem->redir.in 
		|| add_address(&elem->p_data->collector, elem->redir.in) == 1)
		return (-1);
	while (elem->fullcmd[elem->i] 
		&& contains(elem->fullcmd[elem->i], " \t\n\r\v\f"))
		elem->i++;
	while (elem->fullcmd[elem->i] 
		&& !contains(elem->fullcmd[elem->i], " \t\n\r\v\f"))
	{
		i = redir_quote(elem, 0, elem->redir.in);
		if (elem->i == (int)ft_strlen(elem->fullcmd))
			break ;
		if (i != 0 && i != 127)
			return (i);
		if ((elem->fullcmd[elem->i] == '<' || elem->fullcmd[elem->i] == '>')
			&& (elem->fullcmd[elem->i - 1] != '\\'))
		{
			elem->i--;
			break ;
		}
		if (i != 127)
			elem->redir.in[++elem->redir.i] = elem->fullcmd[elem->i];
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
		&& !contains(elem->fullcmd[elem->i], " \t\n\r\v\f"))
	{
		i = redir_quote(elem, 1, elem->redir.out1);
		if (elem->i == (int)ft_strlen(elem->fullcmd))
			break ;
		if (i != 0 && i != 127)
			return (i);
		if ((elem->fullcmd[elem->i] == '<' || elem->fullcmd[elem->i] == '>')
			&& (elem->fullcmd[elem->i - 1] != '\\'))
		{
			elem->i--;
			break ;
		}
		if (i != 127)
			elem->redir.out1[++elem->redir.i] = elem->fullcmd[elem->i];
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
		&& !contains(elem->fullcmd[elem->i], " \t\n\r\v\f"))
	{
		i = redir_quote(elem, 2, elem->redir.out2);
		if (elem->i == (int)ft_strlen(elem->fullcmd))
			break ;
		if (i != 0 && i != 127)
			return (i);
		if ((elem->fullcmd[elem->i] == '<' || elem->fullcmd[elem->i] == '>')
			&& (elem->fullcmd[elem->i - 1] != '\\'))
		{
			elem->i--;
			break ;
		}
		if (i != 127)
			elem->redir.out2[++elem->redir.i] = elem->fullcmd[elem->i];
		elem->i++;
	}
	if (i != 127)
		elem->redir.out2[elem->redir.i + 1] = 0;
	if (file_create(elem, 2) == -1)
		return (-1);
	return (1);
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
		if ((elem->redir.end == 0 && (elem->i == 2 || (elem->i > 3 && elem->fullcmd[elem->i - 2] == '2' && contains(elem->fullcmd[elem->i - 3], " \t\n\r\v\f")))) || (elem->redir.end == 1 && (elem->i == 3 || (elem->i > 4 && elem->fullcmd[elem->i - 3] == '2' && (contains(elem->fullcmd[elem->i - 4], " \t\n\r\v\f"))))))
			i = redir_out_err(elem, 0);
		else
			i = redir_out(elem, 0);
	}
	else if (elem->fullcmd[elem->i] == '<')
		i = redir_in(elem, 0);
	if (elem->i >= (int)ft_strlen(elem->fullcmd))
		return (-3);
	return (i);
}
