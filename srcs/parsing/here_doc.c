/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 23:20:02 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/04 18:22:28 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//HERE DOC GESTION

int		hd_single_quotes(t_parse *elem, char *hd)
{	
	if (elem->i == ((int)ft_strlen(elem->fullcmd) - 1))
		return (quotes_error('\''));
	if (elem->fullcmd[elem->i + 1] == '\'' && (elem->fullcmd[elem->i + 2] == ' ' || elem->fullcmd[elem->i + 2] == 0))
		hd[++elem->j] = 0;
	while (elem->fullcmd[elem->i] && elem->fullcmd[++elem->i] != '\'')
		hd[++elem->j] = elem->fullcmd[elem->i];
	if ((elem->i == (int)ft_strlen(elem->fullcmd)) && elem->fullcmd[elem->i] != '\'')
		return (quotes_error('\''));
	elem->i++;
	return (-2);
}

int	hd_double_quotes(t_parse *elem, char *hd)
{
	if (elem->i == ((int)ft_strlen(elem->fullcmd) - 1))
		return (quotes_error('\"'));
	while (elem->fullcmd[elem->i] && elem->fullcmd[++elem->i] != '"')
	{
		if (elem->fullcmd[elem->i] == '\\')
			if (elem->fullcmd[elem->i + 1] == '$' || elem->fullcmd[elem->i + 1] == '\\' || elem->fullcmd[elem->i + 1] == '"')
				elem->i++;
		hd[++elem->j] = elem->fullcmd[elem->i];
	}
	if ((elem->i == (int)ft_strlen(elem->fullcmd)) && elem->fullcmd[elem->i] != '"')
		return (quotes_error('\"'));
	elem->i++;
	if (hd[0] == '\0' && (elem->fullcmd[elem->i] == ' ' || elem->fullcmd[elem->i] == '\0'))
		return (1);
	return (-2);
}

int		hd_quotes_handler(t_parse *elem, char *hd)
{	
	int	ret;

	ret = -2;
	while (elem->fullcmd[elem->i] == '\'' || elem->fullcmd[elem->i] == '"')
	{
		while (elem->fullcmd[elem->i] == '"')
		{
			ret = hd_double_quotes(elem, hd);
			if (ret == -1)
				return (-1);
		}
		while (elem->fullcmd[elem->i] == '\'')
		{
			ret = hd_single_quotes(elem, hd);
			if (ret == -1)
				return (-1);
		}
	}
	if (elem->fullcmd[elem->i] == '\\')
		elem->i++;
	return (ret);
}

char	*init_hd(t_parse *elem)
{
	static int	nb = 0;
	
	if (elem->redir.hd == NULL)
	{
		elem->redir.hd = malloc(sizeof(char *) * ft_strlen(elem->fullcmd) + 1);
		elem->redir.hd[0] = NULL;
	}
	if (elem->redir.hd == NULL || add_address(&elem->p_data->collector, elem->redir.hd) == 1)
		return (NULL);
	if (elem->redir.hd[nb] == NULL)
		elem->redir.hd[nb] = malloc(sizeof(char) * ft_strlen(elem->fullcmd) + 1);
	if (elem->redir.hd[nb] == NULL || add_address(&elem->p_data->collector, elem->redir.hd[nb]) == 1)
		return (NULL);
	nb++;
	elem->redir.hd[nb] = NULL;
	elem->j = -1;
	return (elem->redir.hd[nb - 1]);
}

int	here_doc(t_parse *elem)
{	
	int		ret;
	char	*hd;
	
	// redir_reset(elem, 15);
	hd = init_hd(elem);
	if (hd == NULL)
		return (-1);
	elem->i += 2;
	while (contains(elem->fullcmd[elem->i], " \t\n\r\v\f"))
		elem->i++;
	while (elem->fullcmd[elem->i] && elem->fullcmd[elem->i] != ' ')
	{
		ret = hd_quotes_handler(elem, hd);
		if (ret == -1)
			return (-1);
		if ((elem->fullcmd[elem->i] == ' ' && elem->fullcmd[elem->i - 1] != '\\') && (hd[0] || (!hd[0] && (elem->fullcmd[elem->i - 1] == '"' || elem->fullcmd[elem->i - 1] == '\'') && (elem->fullcmd[elem->i - 2] == '"' || elem->fullcmd[elem->i - 2] == '\'' || ret == 1))))
			break ;
		hd[++elem->j] = elem->fullcmd[elem->i];
		if (elem->i < (int)ft_strlen(elem->fullcmd))
			elem->i++;
	}
	hd[++elem->j] = 0;
	if (hd == NULL)
		return (-1);
	elem->j = -1;
	return (1);
}
