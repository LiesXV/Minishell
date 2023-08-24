/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:02:59 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/08/24 02:51:36 by lmorel           ###   ########.fr       */
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
		free(tmp->address);
		free(tmp);
	}
	*lst = NULL;
}
