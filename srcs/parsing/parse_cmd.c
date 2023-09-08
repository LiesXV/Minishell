/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 17:40:27 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/08 19:23:19 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	single_quotes(t_parse *elem)
{
	if (elem->i == ((int)ft_strlen(elem->fullcmd) - 1))
		return (quotes_error('\''));
	if (elem->fullcmd[elem->i + 1] == '\'' 
		&& (contains(elem->fullcmd[elem->i + 2], " \t\n\r\v\f") 
			|| elem->fullcmd[elem->i + 2] == 0))
		elem->cmd[++elem->j] = 0;
	while (elem->fullcmd[elem->i] && elem->fullcmd[++elem->i] != '\'')
		elem->cmd[++elem->j] = elem->fullcmd[elem->i];
	if ((elem->i == (int)ft_strlen(elem->fullcmd)) 
		&& elem->fullcmd[elem->i] != '\'')
		return (quotes_error('\''));
	elem->i++;
	return (-2);
}

int	double_quotes(t_parse *elem)
{
	int	ret;

	if (elem->i == ((int)ft_strlen(elem->fullcmd) - 1))
		return (quotes_error('\"'));
	while (elem->fullcmd[elem->i] && elem->fullcmd[++elem->i] != '"')
	{
		ret = 0;
		if (elem->fullcmd[elem->i] == '$' && elem->fullcmd[elem->i - 1] != '\\')
			ret = var_handler(elem, 0, 0, 0);
		if (elem->fullcmd[elem->i] == '\\')
		{
			if (elem->fullcmd[elem->i + 1] == '$' || elem->fullcmd[elem->i + 1] == '\\' || elem->fullcmd[elem->i + 1] == '"')
				elem->i++;
		}
		if (ret != 1)
			elem->cmd[++elem->j] = elem->fullcmd[elem->i];
	}
	if (elem->fullcmd[elem->i] == '"' && (contains(elem->fullcmd[elem->i + 1], " \t\n\r\v\f") || elem->fullcmd[elem->i + 1] == '\0') && !elem->cmd[0])
		elem->cmd[0] = '\0';
	if ((elem->i == (int)ft_strlen(elem->fullcmd)) 
		&& elem->fullcmd[elem->i] != '"')
		return (quotes_error('\"'));
	elem->i++;
	if (elem->cmd[0] == '\0' && (contains(elem->fullcmd[elem->i], " \t\n\r\v\f") || elem->fullcmd[elem->i] == '\0'))
		return (1);
	return (-2);
}

int	quotes_handler(t_parse *elem)
{
	int	ret;

	ret = -2;
	while (elem->fullcmd[elem->i] == '\'' || elem->fullcmd[elem->i] == '"')
	{
		while (elem->fullcmd[elem->i] == '"')
		{
			ret = double_quotes(elem);
			if (ret == -1)
				return (-1);
		}
		while (elem->fullcmd[elem->i] == '\'')
		{
			ret = single_quotes(elem);
			if (ret == -1)
				return (-1);
		}
	}
	if (elem->fullcmd[elem->i] == '\\')
		elem->i++;
	return (ret);
}

int	cmd_redir(t_parse *elem, int ret)
{
	if (elem->fullcmd[elem->i] && elem->fullcmd[elem->i] == '$' 
		&& (elem->i == 0 || elem->fullcmd[elem->i - 1] != '\\'))
	{
		if (elem->fullcmd[elem->i + 1] == '\\')
			elem->cmd[++elem->j] = elem->fullcmd[elem->i];
		else
			ret = var_handler(elem, 0, 0, 1);
	}
	if ((elem->fullcmd[elem->i] == '>' || elem->fullcmd[elem->i] == '<') 
		&& (elem->i == 0 || elem->fullcmd[elem->i - 1] != '\\'))
		ret = redir(elem, 0);
	return (ret);
}

char	*only_cmd(t_parse *elem)
{
	int	ret;

	while (elem->fullcmd[elem->i] && !contains(elem->fullcmd[elem->i], " \t\n\r\v\f") && elem->fullcmd[elem->i] != '|')
	{
		ret = -2;
		if ((elem->fullcmd[elem->i] == '1' || elem->fullcmd[elem->i] == '2') 
			&& elem->fullcmd[elem->i + 1] == '>' 
			&& (!elem->cmd[0] || contains(elem->fullcmd[elem->i - 1], " \t\n\r\v\f")))
			elem->i++;
		ret = quotes_handler(elem);
		if (ret == -1)
			return (NULL);
		ret = cmd_redir(elem, ret);
		if (ret == -1)
			return (NULL);
		if ((contains(elem->fullcmd[elem->i], " \t\n\r\v\f") && elem->fullcmd[elem->i - 1] != '\\') && (elem->cmd[0] || (!elem->cmd[0] && (elem->fullcmd[elem->i - 1] == '"' || elem->fullcmd[elem->i - 1] == '\'') && (elem->fullcmd[elem->i - 2] == '"' || elem->fullcmd[elem->i - 2] == '\'' || ret == 1))))
			break ;
		if (elem->i < (int)ft_strlen(elem->fullcmd) && ((elem->fullcmd[elem->i] == '$' && (elem->i == 0 || elem->fullcmd[elem->i - 1] == '\\')) || (elem->fullcmd[elem->i] != '$' && ret == -2)))
			elem->cmd[++elem->j] = elem->fullcmd[elem->i];
		if (elem->i < (int)ft_strlen(elem->fullcmd))
			elem->i++;
	}
	elem->cmd[elem->j + 1] = 0;
	return (elem->cmd);
}
