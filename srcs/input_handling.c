/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:27:05 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/06/15 14:39:03 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

int	input_handling(char *input, t_data *data)
{
	char **fullcommands;
	int		i;
	//t_list	*elem;
	//t_list	*prev;
	
	i = 0;
	fullcommands = ft_split(input, '|'); //split toutes les commandes
	if (!fullcommands)
		return (FAILURE);
	while (fullcommands[i] != NULL)
	{
		add_address(&data->collector, fullcommands[i]);
		get_cmd(add_cmd(fullcommands[i], data), data);
		i++;
		wait(NULL);
	}
	add_address(&data->collector, fullcommands);
	return (SUCCESS);
	// data->cmd_lst = elem;
	// prev = elem;
	// i = 0;
	// while (fullcommands[i] != NULL)
	// {
	// 	elem = malloc(sizeof(t_list));
	// 	if (!elem)
	// 		return (FAILURE);
	// 	elem->cmd = ft_split(fullcommands[i]); //split tous les args de cmd
	// 	elem->path = data->path;
	// 	i++;
	// 	elem->next = NULL;
	// 	prev->next = elem;
	// }
}
