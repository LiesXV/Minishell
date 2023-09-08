/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 17:38:35 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/08 20:08:57 by ibenhaim         ###   ########.fr       */
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
		if (elem->fullcmd[elem->i + 1] == '$' || elem->fullcmd[elem->i + 1] == '\\' || elem->fullcmd[elem->i + 1] == '"')
			elem->i++;
	}
	return (err);
}

int		double_quotes_arg(t_parse *elem, int nb)
{
	int err;
	
	if (elem->i == ((int)ft_strlen(elem->fullcmd) - 1))
		return (quotes_error('\"'));
	while (elem->fullcmd[elem->i] && elem->fullcmd[++elem->i] != '"')
	{
		err = 0;
		err = inside_dquote(elem, nb, err);
		if (err != 1)
			elem->args[nb][++elem->j] = elem->fullcmd[elem->i];
	}
	if (elem->fullcmd[elem->i] == '"' && (contains(elem->fullcmd[elem->i + 1], " \t\n\r\v\f") || elem->fullcmd[elem->i + 1] == '\0') && !elem->args[nb][0])
		elem->args[nb][0] = '\0';
	if ((elem->i == (int)ft_strlen(elem->fullcmd)) && elem->fullcmd[elem->i] != '"')
		return (quotes_error('\"'));
	elem->i++;
	if (elem->args[nb][0] == '\0' && (contains(elem->fullcmd[elem->i], " \t\n\r\v\f") || elem->fullcmd[elem->i] == '\0'))
		return (1);
	return (0);
}

int	single_quotes_arg(t_parse *elem, int nb)
{
	if (elem->i == ((int)ft_strlen(elem->fullcmd) - 1))
		return (quotes_error('\''));
	if (elem->fullcmd[elem->i + 1] == '\'' && (contains(elem->fullcmd[elem->i + 2], " \t\n\r\v\f") || elem->fullcmd[elem->i + 2] == 0))
		elem->args[nb][++elem->j] = 0;
	while (elem->fullcmd[elem->i] && elem->fullcmd[++elem->i] != '\'')
		elem->args[nb][++elem->j] = elem->fullcmd[elem->i];
	if ((elem->i == (int)ft_strlen(elem->fullcmd)) && elem->fullcmd[elem->i] != '\'')
		return (quotes_error('\''));
	elem->i++;
	return (1);
}

int	arg_quotes_handler(t_parse *elem, int nb, int err)
{
	if ((elem->fullcmd[elem->i] == '1' || elem->fullcmd[elem->i] == '2') && elem->fullcmd[elem->i + 1] == '>' && (contains(elem->fullcmd[elem->i], " \t\n\r\v\f")))
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
	if (elem->fullcmd[elem->i] == '\\' && elem->i == (int)ft_strlen(elem->fullcmd) - 1 && elem->fullcmd[elem->i - 1] != '\\')
		return (err);
	if (elem->fullcmd[elem->i] == '\\')
		elem->i++;
	return (err);
}

char	*parse_arg(t_parse *elem, int nb)
{
	int	err;
	
	if (init_parse_arg(elem, nb) == FAILURE)
		return (NULL);
	while (elem->fullcmd[elem->i] && elem->fullcmd[++elem->i] && (!contains(elem->fullcmd[elem->i], " \t\n\r\v\f") || (contains(elem->fullcmd[elem->i], " \t\n\r\v\f") && elem->fullcmd[elem->i - 1] == '\\')))
	{
		if (elem->fullcmd[elem->i] == '|' && elem->fullcmd[elem->i - 1] != '\\' && elem->j == -1)
		{
			elem->args[nb][++elem->j] = elem->fullcmd[elem->i];
			elem->args[nb][++elem->j] = 0;
			elem->i++;
			break;
		}
		if (elem->fullcmd[elem->i] == '|' && elem->fullcmd[elem->i - 1] != '\\')
			break ;
		err = 0;
		err = arg_quotes_handler(elem, nb, err);
		if (err == -1)
			return (NULL);
		err = arg_redir(elem, err, nb);
		if (err == -3)
		{
			elem->args[nb][elem->j + 1] = 0;
			return (elem->args[nb]);
		}
		if ((contains(elem->fullcmd[elem->i], " \t\n\r\v\f") && elem->fullcmd[elem->i - 1] != '\\') && (elem->args[nb][0] || (!elem->args[nb][0] && (elem->fullcmd[elem->i - 1] == '"' || elem->fullcmd[elem->i - 1] == '\'') && (elem->fullcmd[elem->i - 2] == '"' || elem->fullcmd[elem->i - 2] == '\'' || err == 1))))
			break ;
		if (elem->fullcmd[elem->i] && (!contains(elem->fullcmd[elem->i], " \t\n\r\v\f") || (contains(elem->fullcmd[elem->i], " \t\n\r\v\f") && elem->fullcmd[elem->i - 1] == '\\')) && /*err != 1 &&*/ err != 4 && ((elem->fullcmd[elem->i] == '$' && elem->fullcmd[elem->i - 1] != '$' && elem->fullcmd[elem->i + 1] == 0) || (elem->fullcmd[elem->i] == '$' && elem->fullcmd[elem->i - 1] == '\\') || (elem->fullcmd[elem->i] != '$')))
			elem->args[nb][++elem->j] = elem->fullcmd[elem->i];
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
		if (elem->tmp)
			free(elem->tmp);
		elem->arg = parse_arg(elem, i);
		if (elem->arg == NULL)
			return (FAILURE);
		if (arg_is_blank(elem->arg, elem))
			elem->arg = parse_arg(elem, ++i);
		if (!elem->arg || (!elem->arg[0] && !elem->fullcmd[elem->i]))
		{
			free(elem->arg);
			break ;
		}
		i++;
	}
	elem->args[i] = NULL;
	return (SUCCESS);
}

int	parse(t_parse *elem)
{
	elem->rlist = malloc(sizeof(t_redir *));
	if (!elem->rlist || add_address(&elem->p_data->collector, elem->rlist) == 1)
		return (FAILURE);
	*elem->rlist = NULL;
	elem->redir.sstdin = 0;
	elem->redir.sstdout = 1;
	elem->redir.sstderr = 2;
	elem->redir.out1 = NULL;
	elem->redir.out2 = NULL;
	elem->redir.in = NULL;
	elem->redir.hd = NULL;
	elem->redir.next = NULL;
	elem->cmd = malloc(sizeof(char) * (ft_strlen(elem->fullcmd) + 1));
	if (!elem->cmd)
		return (FAILURE);
	elem->cmd[0] = 0;
	elem->i = 0;
	while (elem->fullcmd[elem->i] && contains(elem->fullcmd[elem->i], " \t\n\r\v\f"))
		elem->i++;
	elem->j = -1;
	elem->cmd = only_cmd(elem);
	if (!elem->cmd)
		return (FAILURE);
	// only ./minishell or more programs ?
	if (!ft_strcmp(elem->cmd, "./minishell"))
			init_signals(1);
	elem->args = malloc(sizeof(char *) * 1);
	if (!elem->args || form_args(elem) == FAILURE || add_address(&elem->p_data->collector, elem->cmd) == 1 || add_tab_to_gb(elem, elem->args) == 1)
		return (FAILURE);
	rlist_add_back(elem->rlist, new_rlist_elem(elem));
	return (SUCCESS);
}
