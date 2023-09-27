/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:27:05 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/27 16:34:27 by ibenhaim         ###   ########.fr       */
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
	int		i;

	fullcommands = NULL;
	fullcommands = trixsplit(input, ';');
	if (!fullcommands)
		return (FAILURE);
	i = 0;
	while (fullcommands[i])
	{
		if (add_address(&data->collector, fullcommands[i]) == 1)
			return (FAILURE);
		i++;
	}
	add_address(&data->collector, fullcommands);
	data->cmd_lst = formating(fullcommands, data);
	if (data->cmd_lst == NULL)
		return (FAILURE);
	while ((*data->cmd_lst))
	{
		handle_exec(data);
		(*data->cmd_lst) = (*data->cmd_lst)->next;
	}
	return (SUCCESS);
}
