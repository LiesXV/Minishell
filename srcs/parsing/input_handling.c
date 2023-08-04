/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:27:05 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/08/04 06:47:50 by ibenhaim         ###   ########.fr       */
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
	int		i;

	fullcommands = trixsplit(input, ';'); //split toutes les commandes
	if (!fullcommands)
		return (FAILURE);
	i = 0;
	while (fullcommands[i])
		add_address(&data->collector, fullcommands[i++]);
	data->cmd_lst = formating(fullcommands, data);
	if (data->cmd_lst == NULL)
		return (FAILURE);
	// printlist(&(*data->cmd_lst));
	while ((*data->cmd_lst))
	{
		handle_exec(data);
		(*data->cmd_lst) = (*data->cmd_lst)->next;
	}
	free(fullcommands);
	return (SUCCESS);
}
