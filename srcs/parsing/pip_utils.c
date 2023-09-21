/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pip_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 19:46:34 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/21 16:26:41 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	redir_utils(t_parse *elem, char *dest, int i)
{
	if (elem->i == (int)ft_strlen(elem->fullcmd))
		return (1);
	if ((elem->fullcmd[elem->i] == '<' || elem->fullcmd[elem->i] == '>')
		&& (elem->fullcmd[elem->i - 1] != '\\'))
	{
		elem->i--;
		return (1);
	}
	if (i != 127)
		dest[++elem->redir.i] = elem->fullcmd[elem->i];
	return (0);
}

void	pip_add_back(t_piplist **lst, t_piplist *new)
{
	t_piplist	*cur;

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

int	pip_redir_util(t_redir *cur, char *str, int i)
{
	if (cur->out1 && i == 1 
		&& ft_strnstr(str, cur->out1, ft_strlen(str)) 
		&& (ft_strnstr(str, ">", ft_strlen(str)) 
			|| ft_strnstr(str, ">>", ft_strlen(str))))
	{
		cur->out1 = NULL;
		return (cur->sstdout);
	}
	if (cur->out2 && i == 2 
		&& ft_strnstr(str, cur->out2, ft_strlen(str)) 
		&& ft_strnstr(str, "2>", ft_strlen(str)))
	{
		cur->out2 = NULL;
		return (cur->sstderr);
	}
	return (i);
}

t_redir	create_pip_redir(char *str, t_parse *elem)
{
	t_redir	*cur;
	t_redir	red;

	red.sstdin = 0;
	red.sstdout = 1;
	red.sstderr = 2;
	red.hd = NULL;
	cur = *elem->rlist;
	while (cur)
	{
		if (cur->in && red.sstdin == 0 
			&& ft_strnstr(str, cur->in, ft_strlen(str)) 
			&& ft_strnstr(str, "<", ft_strlen(str)))
		{
			red.sstdin = cur->sstdin;
			cur->in = NULL;
		}
		red.sstdout = pip_redir_util(cur, str, red.sstdout);
		red.sstderr = pip_redir_util(cur, str, red.sstderr);
		cur = cur->next;
	}
	return (red);
}

int	valid_pip(char c, char *fullcmd)
{
	int	i;

	i = 0;
	while (fullcmd[i])
	{
		i = error_in_quotes(fullcmd, i);
		if (i == (int)ft_strlen(fullcmd))
			break ;
		if (fullcmd[i] && fullcmd[i] == '\\')
			i++;
		if (i == (int)ft_strlen(fullcmd))
			break ;
		if (fullcmd[i] && fullcmd[i] == c && fullcmd[i - 1] != '\\')
		{
			if (fullcmd[i + 1] && only_spaces(fullcmd + i + 1))
				return (pipe_error('|'), -1);
			else if (!contains('|', fullcmd + i + 1))
				return (i);
		}
		i++;
	}
	return (-1);
}
