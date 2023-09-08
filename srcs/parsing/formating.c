/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   formating.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 16:01:54 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/07 19:29:45 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void printlist(t_parse **head)
{
	t_parse *cur;
	t_piplist *nav;
	t_redir	*red;
	int i;
	
	cur = *head;
	while (cur)
	{
		i = 0;
		ft_printf("\x1B[35m");
		ft_printf("---\tfullcmd : %s\n", cur->fullcmd);
		ft_printf("---\tonly cmd: %s\n", cur->cmd);
		ft_printf("---\there doc: ");
		if (cur->redir.hd)
		{
			while (cur->redir.hd[i] != NULL)
				ft_printf("%s, ", cur->redir.hd[i++]);
		}
		ft_printf("\n");
		i = 0;
		ft_printf("---\tlast redir : stdin: %s (%d), stdout1: %s (%d), stdout2: %s (%d)\n", cur->redir.in, cur->redir.sstdin, cur->redir.out1, cur->redir.sstdout, cur->redir.out2, cur->redir.sstderr);
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
			ft_printf("\n\n");
		}
		if (cur->rlist)
		{
			red = *cur->rlist;
			ft_printf("---\tredirs : \n");
			while (red)
			{
				i = 0;
				ft_printf("---\t\tstdin: %s -> %d, stdout: %s -> %d, stderr: %s -> %d\n", red->in, red->sstdin, red->out1, red->sstdout, red->out2, red->sstderr);
				red = red->next;
			}
			ft_printf("\n");
		}
		if (cur->piplist)
		{
			nav = *cur->piplist;
			while (nav)
			{
				ft_printf("---\t\tpiplist path : %s\n", nav->path);
				i = 0;
				ft_printf("---\t\there doc: ");
				if (nav->redir.hd)
				{
					while (nav->redir.hd[i] != NULL)
						ft_printf("%s, ", nav->redir.hd[i++]);
				}
				ft_printf("\n");
				ft_printf("---\t\tstdin: %d, stdout: %d, stderr: %d\n", nav->redir.sstdin, nav->redir.sstdout, nav->redir.sstderr);
				i = 0;
				while (nav->cmd[i])
				{
					ft_printf("---\t\tpiplist cmd arg %d : %s\n", i, nav->cmd[i]);
					i++;
				}
				nav = nav->next;
				ft_printf("\n");
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
	if (!head || add_address(&data->collector, head) == 1)
		return (NULL);
	*head = NULL;
	while (cmds[i]!= NULL)
	{
		new = malloc(sizeof(t_parse));
		if (!new || add_address(&data->collector, new) == 1)
			return (NULL);
		new->p_data = data;
		new->fullcmd = cmds[i];
		new->args = NULL;
		new->next = NULL;
		if (parse(new) == FAILURE)
			return (NULL);
		if (contains('|', new->fullcmd))
		{	
			if (handle_pipes(new) == FAILURE)
				return (NULL);
		}
		else
			new->piplist = NULL;
		parse_add_back(head, new);
		i++;
	}
	if (DEBUG == 1) printlist(head);
	return (head);
}
