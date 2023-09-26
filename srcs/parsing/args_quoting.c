/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_quoting.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 19:48:11 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/08 19:56:56 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	inside_dquote(t_parse *elem, int nb, int err)
{
	if (elem->fullcmd[elem->i] == '$' && elem->fullcmd[elem->i - 1] != '\\')
	{
		err = var_handler(elem, 1, nb, 0);
		if (err == -2)
		{
			elem->i--;
			err = 1;
		}
	}
	if (elem->fullcmd[elem->i] == '\\')
	{
		if (elem->fullcmd[elem->i + 1] == '$'
			|| elem->fullcmd[elem->i + 1] == '\\'
			|| elem->fullcmd[elem->i + 1] == '"')
			elem->i++;
	}
	return (err);
}

int	double_quotes_arg(t_parse *elem, int nb)
{
	int	err;

	if (elem->i == ((int)ft_strlen(elem->fullcmd) - 1))
		return (quotes_error('\"'));
	while (elem->fullcmd[elem->i] && elem->fullcmd[++elem->i] != '"')
	{
		err = 0;
		err = inside_dquote(elem, nb, err);
		if (err != 1)
			elem->args[nb][++elem->j] = elem->fullcmd[elem->i];
	}
	if (elem->fullcmd[elem->i] == '"'
		&& (contains(elem->fullcmd[elem->i + 1], " \t\n\r\v\f")
			|| elem->fullcmd[elem->i + 1] == '\0') && !elem->args[nb][0])
		elem->args[nb][0] = '\0';
	if ((elem->i == (int)ft_strlen(elem->fullcmd))
		&& elem->fullcmd[elem->i] != '"')
		return (quotes_error('\"'));
	elem->i++;
	if (elem->args[nb][0] == '\0'
		&& (contains(elem->fullcmd[elem->i], " \t\n\r\v\f")
		|| elem->fullcmd[elem->i] == '\0'))
		return (1);
	return (0);
}

int	single_quotes_arg(t_parse *elem, int nb)
{
	if (elem->i == ((int)ft_strlen(elem->fullcmd) - 1))
		return (quotes_error('\''));
	if (elem->fullcmd[elem->i + 1] == '\''
		&& (contains(elem->fullcmd[elem->i + 2], " \t\n\r\v\f")
			|| elem->fullcmd[elem->i + 2] == 0))
		elem->args[nb][++elem->j] = 0;
	while (elem->fullcmd[elem->i] && elem->fullcmd[++elem->i] != '\'')
		elem->args[nb][++elem->j] = elem->fullcmd[elem->i];
	if ((elem->i == (int)ft_strlen(elem->fullcmd))
		&& elem->fullcmd[elem->i] != '\'')
		return (quotes_error('\''));
	elem->i++;
	return (1);
}

int	quotes_handler_util(t_parse *elem, int err)
{
	if (elem->fullcmd[elem->i] == '\\'
		&& elem->i == (int)ft_strlen(elem->fullcmd) - 1
		&& elem->fullcmd[elem->i - 1] != '\\')
		return (err);
	if (elem->fullcmd[elem->i] == '\\')
		elem->i++;
	return (-99);
}

int	arg_quotes_handler(t_parse *elem, int nb, int err)
{
	if ((elem->fullcmd[elem->i] == '1'
			|| elem->fullcmd[elem->i] == '2')
		&& elem->fullcmd[elem->i + 1] == '>'
		&& (contains(elem->fullcmd[elem->i], " \t\n\r\v\f")))
		elem->i++;
	while (elem->fullcmd[elem->i] == '\'' || elem->fullcmd[elem->i] == '"')
	{
		while (elem->fullcmd[elem->i] == '"')
		{
			err = double_quotes_arg(elem, nb);
			if (err == -1)
				return (-1);
		}
		while (elem->fullcmd[elem->i] == '\'')
		{
			err = single_quotes_arg(elem, nb);
			if (err == -1)
				return (-1);
		}
	}
	if (quotes_handler_util(elem, err) != -99)
		return (err);
	return (err);
}
