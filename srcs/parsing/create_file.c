/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 17:49:36 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/23 10:27:40 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	rlist_add_back(t_redir **lst, t_redir *new)
{
	t_redir	*cur;

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

t_redir	*new_rlist_elem(t_parse *elem)
{
	t_redir	*new;

	new = malloc(sizeof(t_redir));
	if (!new || add_address(&elem->p_data->collector, new) == 1)
		return (NULL);
	new->in = elem->redir.in;
	new->out1 = elem->redir.out1;
	new->out2 = elem->redir.out2;
	new->hd = elem->redir.hd;
	new->sstdin = elem->redir.sstdin;
	new->sstdout = elem->redir.sstdout;
	new->sstderr = elem->redir.sstderr;
	new->next = NULL;
	return (new);
}

int	file_out2_create(t_parse *elem)
{
	if (!elem->redir.out2[0])
	{
		g_end_status = 127;
		return (error_exit(elem->redir.out2, "No such file or directory\n"));
	}
	if (elem->redir.end == 1)
		elem->redir.sstderr = \
		open(elem->redir.out2, O_CREAT | O_RDWR | O_APPEND, 0644);
	else
		elem->redir.sstderr = \
		open(elem->redir.out2, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (elem->redir.sstderr == -1)
	{
		g_end_status = 127;
		return (error_exit(elem->redir.out2, "No such file or directory\n"));
	}
	return (0);
}

int	file_in_create(t_parse *elem)
{
	elem->redir.sstdin = open(elem->redir.in, O_RDONLY);
	if (!elem->redir.in[0] || elem->redir.sstdin == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(elem->redir.in, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		g_end_status = 127;
		return (-1);
	}
	g_end_status = 1;
	return (0);
}

int	file_create(t_parse *elem, int type)
{
	errno = 0;
	if (type == 0)
		return (file_in_create(elem));
	if (type == 1)
	{
		if (!elem->redir.out1[0])
		{
			g_end_status = 127;
			return (error_exit(elem->redir.out1, "No such file or directory"));
		}
		if (elem->redir.end == 1)
			elem->redir.sstdout = \
			open(elem->redir.out1, O_CREAT | O_RDWR | O_APPEND, 0644);
		else
			elem->redir.sstdout = \
			open(elem->redir.out1, O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (elem->redir.sstdout == -1)
		{
			g_end_status = 127;
			return (error_exit(elem->redir.out1, "No such file or directory"));
		}
	}
	if (type == 2)
		return (file_out2_create(elem));
	return (0);
}
