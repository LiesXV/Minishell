/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   formating.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 16:01:54 by lmorel            #+#    #+#             */
/*   Updated: 2023/08/17 00:15:06 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void printlist(t_parse **head)
{
	t_parse *cur;
	t_piplist *nav;
	int i;
	
	cur = *head;
	while (cur)
	{
		i = 0;
		ft_printf("\x1B[35m");
		ft_printf("---\tfullcmd : %s\n", cur->fullcmd);
		ft_printf("---\tonly cmd: %s\n", cur->cmd);
		ft_printf("---\there doc: %s\n", cur->redir.hd);
		ft_printf("---\tstdin: %s, stdout1: %s, stdout2: %s\n", cur->redir.in, cur->redir.out1, cur->redir.out2);
		if (cur->args == NULL)
			printf("---\targs   : NULL\n");
		else 
		{
			ft_printf("---\targs : ");
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
				i = 0;
				ft_printf("---\t\tpiplist path : %s\n", nav->path);
				while (nav->cmd[i])
				{
					ft_printf("---\t\tpiplist cmd arg %d : %s\n", i, nav->cmd[i]);
					i++;
				}
				nav = nav->next;
			}
		}
		if (cur->next)
			ft_printf("\n");
		cur = cur->next;
		ft_printf("\x1B[0m");
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

t_parse	**formating(char **cmds, t_data *data)
{
	int		i;
	t_parse	**head;
	t_parse	*new;

	i = 0;
	head = malloc(sizeof(t_parse *));
	if (!head)
		return (NULL);
	add_address(&data->collector, head);
	*head = NULL;
	while (cmds[i]!= NULL)
	{
		new = malloc(sizeof(t_parse));
		if (!new)
			return (NULL);
		add_address(&data->collector, new);
		new->p_data = data;
		new->fullcmd = cmds[i];
		new->args = NULL;
		new->next = NULL;
		if (contains('|', new->fullcmd))
			handle_pipes(new, data);
		else
			new->piplist = NULL;
		if (parse(new) == FAILURE)
			return (NULL);
		parse_add_back(head, new);
		i++;
	}
	if (DEBUG == 1) printlist(head);
	return (head);
}
