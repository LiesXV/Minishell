/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 17:51:39 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/08 19:08:50 by ibenhaim         ###   ########.fr       */
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

int	s_quote_redir(t_parse *elem, char *file)
{
	if (elem->i == ((int)ft_strlen(elem->fullcmd) - 1))
		return (quotes_error('\''));
	if ((elem->fullcmd[elem->i + 1] == '\'' 
			&& contains(elem->fullcmd[elem->i + 2], " \t\n\r\v\f")) && !file)
	{
		file[elem->redir.i] = ' ';
		file[elem->redir.i + 1] = 0;
		elem->i = elem->i + 2;
		return (-1);
	}
	while (elem->fullcmd[elem->i] && elem->fullcmd[++elem->i] != '\'')
		file[++elem->redir.i] = elem->fullcmd[elem->i];
	if ((elem->i == (int)ft_strlen(elem->fullcmd)) 
		&& elem->fullcmd[elem->i] != '\'')
		return (quotes_error('\''));
	file[elem->redir.i + 1] = 0;
	elem->i++;
	if (!contains(elem->fullcmd[elem->i], " \t\n\r\v\f"))
		return (1);
	return (-1);
}

int	quote_util_redir(t_parse *elem, char *file, int std, int ret)
{
	while (elem->fullcmd[elem->i] && elem->fullcmd[++elem->i] != '"')
	{
		ret = 0;
		if (elem->fullcmd[elem->i] == '$' && (elem->i == 0 
				|| elem->fullcmd[elem->i - 1] != '\\'))
		{
			ret = var_redir(elem, std, 0);
			if (ret == -2)
			{
				elem->i--;
				ret = 1;
			}
			if (elem->fullcmd[elem->i] == '"' 
				&& elem->fullcmd[elem->i - 1] != '\\')
				break ;
		}
		if (elem->fullcmd[elem->i] == '\\')
			if (elem->fullcmd[elem->i + 1] == '$' 
				|| elem->fullcmd[elem->i + 1] == '\\' 
				|| elem->fullcmd[elem->i + 1] == '"')
				elem->i++;
		if (ret != 1)
			file[++elem->redir.i] = elem->fullcmd[elem->i];
	}
	return (ret);
}

int	d_quote_redir(t_parse *elem, char *file, int std)
{
	if (elem->i == ((int)ft_strlen(elem->fullcmd) - 1))
		return (quotes_error('"'));
	if ((elem->fullcmd[elem->i + 1] && elem->fullcmd[elem->i + 1] == '"'
			&& contains(elem->fullcmd[elem->i + 2], " \t\n\r\v\f")) && !file)
	{
		file[elem->redir.i] = ' ';
		file[elem->redir.i + 1] = 0;
		elem->i = elem->i + 2;
		return (-1);
	}
	if (quote_util_redir(elem, file, std, 0) != 0 
		&& (elem->i == (int)ft_strlen(elem->fullcmd) - 1))
		return (-1);
	if ((elem->i == (int)ft_strlen(elem->fullcmd)) 
		&& elem->fullcmd[elem->i] != '"')
		return (quotes_error('"'));
	file[elem->redir.i + 1] = 0;
	elem->i++;
	if (!contains(elem->fullcmd[elem->i], " \t\n\r\v\f"))
		return (1);
	return (-1);
}

int	redir_quote(t_parse *elem, int i, char *file)
{
	if (elem->fullcmd[elem->i] == '$' && elem->fullcmd[elem->i - 1] != '\\')
		return (var_redir(elem, i, 1), 127);
	while (elem->fullcmd[elem->i] == '\'' || elem->fullcmd[elem->i] == '\"')
	{
		while (contains(elem->fullcmd[elem->i], "\"'"))
		{
			if (elem->fullcmd[elem->i] == '\"' 
				&& d_quote_redir(elem, file, i) == -1)
			{
				if (file_create(elem, i) == -1)
					return (-1);
				return (4);
			}
			if (elem->fullcmd[elem->i] == '\'' 
				&& s_quote_redir(elem, file) == -1)
			{
				if (file_create(elem, i) == -1)
					return (-1);
				return (4);
			}
		}
	}
	if (elem->fullcmd[elem->i] == '\\')
		elem->i++;
	return (0);
}
