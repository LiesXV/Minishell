/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pip_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 19:46:34 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/08 19:47:52 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		if (cur->in && red.sstdin == 0 && ft_strnstr(str, cur->in, ft_strlen(str)) && ft_strnstr(str, "<", ft_strlen(str)))
		{
			red.sstdin = cur->sstdin;
			cur->in = NULL;
		}
		if (cur->out1 && red.sstdout == 1 && ft_strnstr(str, cur->out1, ft_strlen(str)) && (ft_strnstr(str, ">", ft_strlen(str)) || ft_strnstr(str, ">>", ft_strlen(str))))
		{
			red.sstdout = cur->sstdout;
			cur->out1 = NULL;
		}
		if (cur->out2 && red.sstderr == 2 && ft_strnstr(str, cur->out2, ft_strlen(str)) && ft_strnstr(str, "2>", ft_strlen(str)))
		{
			red.sstderr = cur->sstderr;
			cur->out2 = NULL;
		}	
		cur = cur->next;	
	}
	return (red);
}

int		valid_pip(char c, char *fullcmd)
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
