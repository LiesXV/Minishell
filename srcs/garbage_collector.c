/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:02:59 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/08 18:02:53 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	add_address(t_collector **lst, void *address)
{
	t_collector	*newlst;
	t_collector	*tmp;

	newlst = malloc(sizeof(t_collector));
	if (!newlst)
		return (FAILURE);
	newlst->address = address;
	newlst->next = NULL;
	if (*lst == NULL)
		*lst = newlst;
	else
	{
		tmp = *lst;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = newlst;
	}
	return (SUCCESS);
}

void	free_all(t_collector **lst)
{
	t_collector	*cur;
	t_collector	*tmp;

	cur = *lst;
	while (cur != NULL)
	{
		tmp = cur;
		cur = cur->next;
		if (tmp->address)
			free(tmp->address);
		if (tmp)
			free(tmp);
	}
	*lst = NULL;
}

int	add_tab_to_gb(t_parse *elem, char **args)
{
	int	k;

	if (add_address(&elem->p_data->collector, args) == 1)
		return (1);
	k = 0;
	while (args[k] != NULL)
	{
		if (add_address(&elem->p_data->collector, args[k]) == 1)
			return (1);
		k++;
	}
	return (SUCCESS);
}
