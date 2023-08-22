/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 04:20:49 by lmorel            #+#    #+#             */
/*   Updated: 2023/08/22 23:07:45 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	error_exit(char *name, char *err)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(name, 2);
	if (errno != 0)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
	}
	else
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(err, 2);
		ft_putstr_fd("\n", 2);
	}
	g_end_status = 1;
	return (-1);
}

int	file_out2_create(t_parse *elem)
{
	if (!elem->redir.out2[0])
		return (error_exit(elem->redir.out2, "No such file or directory"));
	if (elem->redir.end == 1)
		elem->redir.sstdout = open(elem->redir.out2, O_CREAT | O_RDWR | O_APPEND, 0644);
	else
		elem->redir.sstdout = open(elem->redir.out2, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (elem->redir.sstderr == -1)
		return (error_exit(elem->redir.out2, "No such file or directory"));
	return (0);
}

int	file_in_create(t_parse *elem)
{
	elem->redir.sstdin = open(elem->redir.in, O_RDONLY);
	if (!elem->redir.in[0] || elem->redir.sstdin == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(elem->redir.in, 2);
		ft_putstr_fd(": No such file or directory", 2);
	}
	g_end_status = 1;
	return (0);
}

int file_create(t_parse *elem, int type)
{
	errno = 0;
	if (elem->redir.sstdout > 2)
		close(elem->redir.sstdout);
	if (elem->redir.sstdin > 2)
		close(elem->redir.sstdin);
	if (type == 0)
		return (file_in_create(elem));
	if (type == 1)
	{
		if (!elem->redir.out1[0])
			return (error_exit(elem->redir.out1, "No such file or directory"));
		if (elem->redir.end == 1)
			elem->redir.sstdout = open(elem->redir.out1, O_CREAT | O_RDWR | O_APPEND, 0644);
		else
			elem->redir.sstdout = open(elem->redir.out1, O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (elem->redir.sstdout == -1)
			return (error_exit(elem->redir.out1, "No such file or directory"));
	}
	if (type == 2)
		return (file_out2_create(elem));
	return (0);
}

int s_quote_redir(t_parse *elem, char *file)
{
	if (elem->i == ((int)ft_strlen(elem->fullcmd) - 1))
		return (quotes_error('\''));
	if ((elem->fullcmd[elem->i + 1] == '\'' && elem->fullcmd[elem->i + 2] == ' ') && !file)
	{
		file[elem->redir.i] = ' ';
		file[elem->redir.i + 1] = 0;
		elem->i = elem->i + 2;
		return (-1);
	}
	while (elem->fullcmd[elem->i] && elem->fullcmd[++elem->i] != '\'')
		file[++elem->redir.i] = elem->fullcmd[elem->i];
	if ((elem->i == (int)ft_strlen(elem->fullcmd)) && elem->fullcmd[elem->i] != '\'')
		return (quotes_error('\''));
	file[elem->redir.i + 1] = 0;
	elem->i++;
	if (elem->fullcmd[elem->i] != ' ')
		return (1);
	return (-1);
}

int quote_util_redir(t_parse *elem, char *file, int std, int ret)
{
	while (elem->fullcmd[elem->i] && elem->fullcmd[++elem->i] != '"')
	{
	ret = 0;
	if (elem->fullcmd[elem->i] == '$' && (elem->i == 0 || elem->fullcmd[elem->i - 1] != '\\'))
	{
		ret = var_redir(elem, std, 0);
		if (ret == -2)
		{
			elem->i--;
			ret = 1;
		}
		if (elem->fullcmd[elem->i] == '"' && elem->fullcmd[elem->i - 1] != '\\')
			break ;
	}
	if (elem->fullcmd[elem->i] == '\\')
	{
		if (elem->fullcmd[elem->i + 1] == '$' || elem->fullcmd[elem->i + 1] == '\\' || elem->fullcmd[elem->i + 1] == '"')
			elem->i++;
	}
	if (ret != 1)
		file[++elem->redir.i] = elem->fullcmd[elem->i];
	}
	return (ret);
}

int d_quote_redir(t_parse *elem, char *file, int std)
{
	if (elem->i == ((int)ft_strlen(elem->fullcmd) - 1))
		return (quotes_error('"'));
	if ((elem->fullcmd[elem->i + 1] && elem->fullcmd[elem->i + 1] == '"' && elem->fullcmd[elem->i + 2] == ' ') && !file)
	{
		file[elem->redir.i] = ' ';
		file[elem->redir.i + 1] = 0;
		elem->i = elem->i + 2;
		return (-1);
	}
	if (quote_util_redir(elem, file, std, 0) != 0 && (elem->i == (int)ft_strlen(elem->fullcmd) - 1))
		return (-1);
	if ((elem->i == (int)ft_strlen(elem->fullcmd)) && elem->fullcmd[elem->i] != '"')
		return (quotes_error('"'));
	file[elem->redir.i + 1] = 0;
	elem->i++;
	if (elem->fullcmd[elem->i] != ' ')
		return (1);
	return (-1);
}

int	redir_quote(t_parse *elem, int i, char *file)
{
	if (elem->fullcmd[elem->i] == '$' && elem->fullcmd[elem->i - 1] != '\\')
		var_redir(elem, i, 1);
	while (elem->fullcmd[elem->i] == '\'' || elem->fullcmd[elem->i] == '\"')
	{
		while (elem->fullcmd[elem->i] == '"')
			if (d_quote_redir(elem, file, i) == -1)
			{
				if (file_create(elem, i) == -1)
					return (-1);
				return (4);
			}
		while (elem->fullcmd[elem->i] == '\'')
			if (s_quote_redir(elem, file) == -1)
			{
				if (file_create(elem, i) == -1)
					return (-1);
				return (4);
			}
	}
	if (elem->fullcmd[elem->i] == '\\')
		elem->i++;
	return (0);
}

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

t_redir *new_rlist_elem(t_parse *elem)
{
	t_redir	*new;

	new = malloc(sizeof(t_redir));
	new->in = elem->redir.in;
	new->out1 = elem->redir.out1;
	new->out2 = elem->redir.out2;
	new->sstdin = elem->redir.sstdin;
	new->sstdout = elem->redir.sstdout;
	new->sstderr = elem->redir.sstderr;
	new->next = NULL;
	return (new);
}

void	redir_reset(t_parse *elem, int i)
{
	if (i == 0)
		elem->i++;
	if ((i == 0 && elem->redir.in != NULL) || (i == 1 && elem->redir.out1 != NULL) || (i == 2 && elem->redir.out2 != NULL))
	{
		rlist_add_back(elem->rlist, new_rlist_elem(elem));
		elem->redir.in = NULL;
		elem->redir.sstdin = 0;
		elem->redir.out1 = NULL;
		elem->redir.sstdout = 1;
		elem->redir.out2 = NULL;
		elem->redir.sstderr = 2;
	}
}

int	redir_in(t_parse *elem, int i)
{
	redir_reset(elem, 0);
	elem->redir.in = malloc(sizeof(char) * ft_strlen(elem->fullcmd) + 1);
	if (!elem->redir.in || add_address(&elem->p_data->collector, elem->redir.in) == -1)
		return (-1);
	while (elem->fullcmd[elem->i] && elem->fullcmd[elem->i] == ' ')
		elem->i++;
	while (elem->fullcmd[elem->i] && elem->fullcmd[elem->i] != ' ')
	{
		i = redir_quote(elem, 0, elem->redir.in);
		if (elem->i == (int)ft_strlen(elem->fullcmd))
			break ;
		if (i != 0)
			return (i);
		if ((elem->fullcmd[elem->i] == '<' || elem->fullcmd[elem->i] == '>') && (elem->fullcmd[elem->i - 1] != '\\'))
		{
			elem->i--;
			break ;
		}
		elem->redir.in[++elem->redir.i] = elem->fullcmd[elem->i];
		elem->i++;
	}
	elem->redir.in[elem->redir.i + 1] = 0;
	if (file_create(elem, 0) == -1)
		return (-1);
	return (1);
}

int redir_out(t_parse *elem, int i)
{
	redir_reset(elem, 1);
	elem->redir.out1 = malloc(sizeof(char) * ft_strlen(elem->fullcmd) + 1);
	if (!elem->redir.out1 || add_address(&elem->p_data->collector, elem->redir.out1) == -1)
		return (-1);
	while (elem->fullcmd[elem->i] && elem->fullcmd[elem->i] == ' ')
		elem->i++;
	while (elem->fullcmd[elem->i] && elem->fullcmd[elem->i] != ' ')
	{
		i = redir_quote(elem, 1, elem->redir.out1);
		if (elem->i == (int)ft_strlen(elem->fullcmd))
			break ;
		if (i != 0)
			return (i);
		if ((elem->fullcmd[elem->i] == '<' || elem->fullcmd[elem->i] == '>') && (elem->fullcmd[elem->i - 1] != '\\'))
		{
			elem->i--;
			break ;
		}
		elem->redir.out1[++elem->redir.i] = elem->fullcmd[elem->i];
		elem->i++;
	}
	elem->redir.out1[elem->redir.i + 1] = 0;
	if (file_create(elem, 1) == -1)
		return (-1);
	return (1);
}

int redir_out_err(t_parse *elem, int i)
{
	redir_reset(elem, 2);
	elem->redir.out2 = malloc(sizeof(char) * ft_strlen(elem->fullcmd) + 1);
	if (!elem->redir.out2 || add_address(&elem->p_data->collector, elem->redir.out2) == -1)
		return (-1);
	while (elem->fullcmd[elem->i] && elem->fullcmd[elem->i] == ' ')
		elem->i++;
	while (elem->fullcmd[elem->i] && elem->fullcmd[elem->i] != ' ')
	{
		i = redir_quote(elem, 2, elem->redir.out2);
		if (elem->i == (int)ft_strlen(elem->fullcmd))
			break ;
		if (i != 0)
			return (i);
		if ((elem->fullcmd[elem->i] == '<' || elem->fullcmd[elem->i] == '>') && (elem->fullcmd[elem->i - 1] != '\\'))
		{
			elem->i--;
			break ;
		}
		elem->redir.out2[++elem->redir.i] = elem->fullcmd[elem->i];
		elem->i++;
	}
	elem->redir.out2[elem->redir.i + 1] = 0;
	if (file_create(elem, 2) == -1)
		return (-1);
	return (1);
}

int	redir(t_parse *elem, int i)
{
	elem->redir.i = -1;
	if (elem->fullcmd[elem->i + 2] && elem->fullcmd[elem->i] == '<' && elem->fullcmd[elem->i + 1] == '<' && elem->fullcmd[elem->i + 2] != '<')
		i = here_doc(elem);
	else if (elem->fullcmd[elem->i] == '>')
	{
		elem->redir.end = 0;
		elem->i++;
		if (elem->fullcmd[elem->i] == '>')
		{
			elem->redir.end = 1;
			elem->i++;
		}
		if ((elem->redir.end == 0 && (elem->i == 2 || (elem->i > 3 && elem->fullcmd[elem->i - 2] == '2' && elem->fullcmd[elem->i - 3] == ' '))) || (elem->redir.end == 1 && (elem->i == 3 || (elem->i > 4 && elem->fullcmd[elem->i - 3] == '2' && (elem->fullcmd[elem->i - 4] == ' ')))))
			i = redir_out_err(elem, 0);
		else
			i = redir_out(elem, 0);
	}
	else if (elem->fullcmd[elem->i] == '<')
		i = redir_in(elem, 0);
	if (elem->i >= (int)ft_strlen(elem->fullcmd))
		return (-3);
	return (i);
}
