/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:27:05 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/07/25 23:34:06 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void free_tab(void **tab)
{
	int	i;
	
	i = 0;
	while (tab[i] !=NULL)
	{
		free(tab[i]);
		i++;
	}
}

void	printtab(char **tab)
{
	int	i = 0;

	while (tab[i] != NULL)
	{
		ft_printf("\t%d : %s\n", i, tab[i]);
		i++;
	}
	return ;
}

int	input_handling(char *input, t_data *data)
{
	char	**fullcommands;

	fullcommands = ft_multisplit(input, ";"); //split toutes les commandes
	if (!fullcommands)
		return (FAILURE);
	data->cmd_lst = formating(fullcommands);
	while ((*data->cmd_lst))
	{
		handle_exec(data);
		(*data->cmd_lst) = (*data->cmd_lst)->next;
	}
	return (SUCCESS);
}