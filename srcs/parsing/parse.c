/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 17:38:35 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/23 19:48:23 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	parse_init(t_parse *elem)
{
	*elem->rlist = NULL;
	elem->hdnb = 0;
	elem->redir.sstdin = 0;
	elem->redir.sstdout = 1;
	elem->redir.sstderr = 2;
	elem->redir.out1 = NULL;
	elem->redir.out2 = NULL;
	elem->redir.in = NULL;
	elem->redir.hd = NULL;
	elem->redir.next = NULL;
}

int	parse(t_parse *elem)
{
	elem->rlist = malloc(sizeof(t_redir *));
	if (!elem->rlist || add_address(&elem->p_data->collector, elem->rlist))
		return (FAILURE);
	parse_init(elem);
	elem->cmd = malloc(sizeof(char) * (ft_strlen(elem->fullcmd) + 1));
	if (!elem->cmd || add_address(&elem->p_data->collector, elem->cmd) == 1)
		return (FAILURE);
	elem->cmd[0] = 0;
	elem->i = 0;
	while (elem->fullcmd[elem->i]
		&& contains(elem->fullcmd[elem->i], " \t\n\r\v\f"))
		elem->i++;
	elem->j = -1;
	elem->cmd = only_cmd(elem);
	if (!elem->cmd)
		return (FAILURE);
	if (!ft_strcmp(elem->cmd, "./minishell"))
		init_signals(1);
	elem->args = malloc(sizeof(char *) * 1);
	if (!elem->args || add_address(&elem->p_data->collector, elem->args)
		|| form_args(elem) == FAILURE)
		return (FAILURE);
	rlist_add_back(elem->rlist, new_rlist_elem(elem));
	return (SUCCESS);
}
