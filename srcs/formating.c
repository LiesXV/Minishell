/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   formating.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 16:01:54 by lmorel            #+#    #+#             */
/*   Updated: 2023/06/23 21:17:41 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void printlist(t_parse **head)
{
	t_parse *cur;
	t_piplist *nav;
	int i;
	
	cur = *head;
	while (cur)
	{
		i = 0;
		ft_printf("\tfullcmd : %s\n", cur->fullcmd);
		ft_printf("\tonly cmd: %s\n", cur->cmd);
		if (cur->args == NULL)
			printf("\targs : NULL\n");
		else 
		{
			ft_printf("\targs : ");
			while (cur->args[i] != NULL)
			{
				ft_printf("%s, ", cur->args[i]);
				i++;
			}
			ft_printf("\n");
		}
		if (cur->piplist)
		{
			nav = *cur->piplist;
			while (nav)
			{
				ft_printf("\t\tpiplist : %s\n", nav->cmd);
				nav = nav->next;
			}
		}
		cur = cur->next;
	}
}

void	parse_add_back(t_parse **lst, t_parse *new)
{
	t_parse	*cur;

	if (lst && *lst)
	{
		cur = *lst;
		while (cur->next)
			cur = cur->next;
		cur->next = new;
	}
	else if (lst)
		*lst = new;
}

t_parse	**formating(char **cmds)
{
	int i = 0;
	t_parse	**head;
	t_parse	*new;
	
	head = malloc(sizeof(t_parse *));
	*head = NULL;
	while (cmds[i]!= NULL)
	{
		new = malloc(sizeof(t_parse));
		new->fullcmd = cmds[i];
		new->args = NULL;
		new->next = NULL;
		parse(new);
		parse_add_back(head, new);
		i++;
	}
	printlist(head);
	return (head);
}