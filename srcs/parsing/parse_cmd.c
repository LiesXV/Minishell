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

int	cmd_util(t_parse *elem, int ret)
{
	if ((contains(elem->fullcmd[elem->i], " \t\n\r\v\f")
			&& elem->fullcmd[elem->i - 1] != '\\')
		&& (elem->cmd[0] || (!elem->cmd[0]
				&& (elem->fullcmd[elem->i - 1] == '"'
					|| elem->fullcmd[elem->i - 1] == '\'')
				&& (elem->fullcmd[elem->i - 2] == '"'
					|| elem->fullcmd[elem->i - 2] == '\'' || ret == 1))))
		return (1);
	if (elem->i < (int)ft_strlen(elem->fullcmd)
		&& ((elem->fullcmd[elem->i] == '$'
				&& (elem->i == 0 || elem->fullcmd[elem->i - 1] == '\\'))
			|| (elem->fullcmd[elem->i] != '$' && ret == -2)))
		elem->cmd[++elem->j] = elem->fullcmd[elem->i];
	if (elem->i < (int)ft_strlen(elem->fullcmd))
		elem->i++;
	return (0);
}

char	*only_cmd(t_parse *elem)
{
	int	ret;

	while (elem->fullcmd[elem->i]
		&& !contains(elem->fullcmd[elem->i], " \t\n\r\v\f")
		&& elem->fullcmd[elem->i] != '|')
	{
		ret = -2;
		if ((elem->fullcmd[elem->i] == '1' || elem->fullcmd[elem->i] == '2')
			&& elem->fullcmd[elem->i + 1] == '>' && (!elem->cmd[0]
				|| contains(elem->fullcmd[elem->i - 1], " \t\n\r\v\f")))
			elem->i++;
		ret = quotes_handler(elem);
		if (ret == -1)
			return (NULL);
		ret = cmd_redir(elem, ret);
		if (ret == -1)
			return (NULL);
		if (cmd_util(elem, ret) == 1)
			break ;
	}
	elem->cmd[elem->j + 1] = 0;
	return (elem->cmd);
}
