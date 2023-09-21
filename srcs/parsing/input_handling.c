/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:27:05 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/21 17:03:14 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	hd_utils(t_parse *elem, char *hd, int ret)
{
	if ((elem->fullcmd[elem->i] == ' ' 
			&& elem->fullcmd[elem->i - 1] != '\\') && (hd[0] || (!hd[0] 
				&& (elem->fullcmd[elem->i - 1] == '"' 
					|| elem->fullcmd[elem->i - 1] == '\'') 
				&& (elem->fullcmd[elem->i - 2] == '"' 
					|| elem->fullcmd[elem->i - 2] == '\'' || ret == 1))))
		return (FAILURE);
	else
		return (SUCCESS);
}

void	free_tab(void **tab)
{
	int	i;

	i = 0;
	while (tab[i] != NULL)
	{
		free(tab[i]);
		i++;
	}
}

int	input_handling(char *input, t_data *data)
{
	char	**fullcommands;

	fullcommands = NULL;
	fullcommands = trixsplit(input, ';');
	if (!fullcommands)
		return (FAILURE);
	data->cmd_lst = formating(fullcommands, data);
	if (data->cmd_lst == NULL)
		return (free_tab((void **)fullcommands), free(fullcommands), FAILURE);
	while ((*data->cmd_lst))
	{
		handle_exec(data);
		(*data->cmd_lst) = (*data->cmd_lst)->next;
	}
	free_tab((void **)fullcommands);
	free(fullcommands);
	return (SUCCESS);
}
