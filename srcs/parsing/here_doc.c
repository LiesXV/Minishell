/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 23:20:02 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/03 01:13:35 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//HERE DOC GESTION

int		hd_single_quotes(t_parse *elem)
{	
	if (elem->i == ((int)ft_strlen(elem->fullcmd) - 1))
		return (quotes_error('\''));
	if (elem->fullcmd[elem->i + 1] == '\'' && (elem->fullcmd[elem->i + 2] == ' ' || elem->fullcmd[elem->i + 2] == 0))
		elem->redir.hd[++elem->j] = 0;
	while (elem->fullcmd[elem->i] && elem->fullcmd[++elem->i] != '\'')
		elem->redir.hd[++elem->j] = elem->fullcmd[elem->i];
	if ((elem->i == (int)ft_strlen(elem->fullcmd)) && elem->fullcmd[elem->i] != '\'')
		return (quotes_error('\''));
	elem->i++;
	return (-2);
}

int	hd_double_quotes(t_parse *elem)
{
	if (elem->i == ((int)ft_strlen(elem->fullcmd) - 1))
		return (quotes_error('\"'));
	while (elem->fullcmd[elem->i] && elem->fullcmd[++elem->i] != '"')
	{
		if (elem->fullcmd[elem->i] == '\\')
			if (elem->fullcmd[elem->i + 1] == '$' || elem->fullcmd[elem->i + 1] == '\\' || elem->fullcmd[elem->i + 1] == '"')
				elem->i++;
		elem->redir.hd[++elem->j] = elem->fullcmd[elem->i];
	}
	if ((elem->i == (int)ft_strlen(elem->fullcmd)) && elem->fullcmd[elem->i] != '"')
		return (quotes_error('\"'));
	elem->i++;
	if (elem->redir.hd[0] == '\0' && (elem->fullcmd[elem->i] == ' ' || elem->fullcmd[elem->i] == '\0'))
		return (1);
	return (-2);
}

int		hd_quotes_handler(t_parse *elem)
{	
	int	ret;

	ret = -2;
	while (elem->fullcmd[elem->i] == '\'' || elem->fullcmd[elem->i] == '"')
	{
		while (elem->fullcmd[elem->i] == '"')
		{
			ret = hd_double_quotes(elem);
			if (ret == -1)
				return (-1);
		}
		while (elem->fullcmd[elem->i] == '\'')
		{
			ret = hd_single_quotes(elem);
			if (ret == -1)
				return (-1);
		}
	}
	if (elem->fullcmd[elem->i] == '\\')
		elem->i++;
	return (ret);
}

int	here_doc(t_parse *elem)
{	
	int ret;
	
	redir_reset(elem, 15);
	elem->i += 2;
	elem->redir.hd = malloc(sizeof(char) * ft_strlen(elem->fullcmd) + 1);
	if (elem->redir.hd == NULL || add_address(&elem->p_data->collector, elem->redir.hd) == 1)
		return (-1);
	while (contains(elem->fullcmd[elem->i], " \t\n\r\v\f"))
		elem->i++;
	while (elem->fullcmd[elem->i] && elem->fullcmd[elem->i] != ' ')
	{
		ret = hd_quotes_handler(elem);
		if (ret == -1)
			return (-1);
		if ((elem->fullcmd[elem->i] == ' ' && elem->fullcmd[elem->i - 1] != '\\') && (elem->redir.hd[0] || (!elem->redir.hd[0] && (elem->fullcmd[elem->i - 1] == '"' || elem->fullcmd[elem->i - 1] == '\'') && (elem->fullcmd[elem->i - 2] == '"' || elem->fullcmd[elem->i - 2] == '\'' || ret == 1))))
			break ;
		elem->redir.hd[++elem->j] = elem->fullcmd[elem->i];
		if (elem->i < (int)ft_strlen(elem->fullcmd))
			elem->i++;
	}
	elem->redir.hd[++elem->j] = 0;
	if (elem->redir.hd == NULL)
		return (-1);
	elem->j = -1;
	return (1);
}
