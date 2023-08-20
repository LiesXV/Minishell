/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 17:38:35 by lmorel            #+#    #+#             */
/*   Updated: 2023/08/20 20:26:33 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int quotes_error(char c)
{
	ft_putstr_fd("minishell: unexpected error while looking for matching '", 2);
	ft_putchar_fd(c, 2);
	ft_putstr_fd("'\n", 2);
	g_end_status = 1;
	return (-1);
}

int pipe_error(char c)
{
	ft_putstr_fd("minishell: unexpected error around '", 2);
	ft_putchar_fd(c, 2);
	ft_putstr_fd("'\n", 2);
	g_end_status = 1;
	return (-1);
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

char **args_to_pip(t_parse *elem)
{
	char	**pip;
	int 	k;
	
	k = 0;
	while (elem->args[k])
		k++;
	pip = malloc(sizeof(char *) * k + 1);
	if (!pip)
		return (NULL);
	k = 0;
	while (elem->args[++elem->j] && elem->args[elem->j][0] != '|')
	{
		pip[k] = elem->args[elem->j];
		k++;
	}
	pip[k] = NULL;
	return (pip);
}

int	handle_pipes(t_parse *elem)
{
	char		**strs;
	int			i;
	t_piplist	*new;

	if (valid_pip('|', elem->fullcmd) > -1)
	{
		elem->piplist = malloc(sizeof(t_piplist *));
		if (!elem->piplist || add_address(&elem->p_data->collector, elem->piplist) == -1)
			return (FAILURE);
		*elem->piplist = NULL;
		strs = NULL;
		strs = trixsplit(elem->fullcmd, '|');
		add_tab_to_gb(elem, strs);
		i = 0;
		elem->j = -1;
		while (strs[i])
		{
			new = malloc(sizeof(t_piplist));
			if (!new || add_address(&elem->p_data->collector, new) == -1)
				return (FAILURE);
			new->cmd = args_to_pip(elem);
			if (!new->cmd || add_address(&elem->p_data->collector, new->cmd) == -1)
				return (FAILURE);
			new->path = get_path(new->cmd[0], elem->p_data);
			if (!new->path || add_address(&elem->p_data->collector, new->path) == -1)
				return (FAILURE);
			new->next = NULL;
			pip_add_back(elem->piplist, new);
			i++;
		}
		return (SUCCESS);
	}
	return (FAILURE);
}

int		single_quotes(t_parse *elem)
{	
	if (elem->i == ((int)ft_strlen(elem->fullcmd) - 1))
		return (quotes_error('\''));
	if (elem->fullcmd[elem->i + 1] == '\'' && (elem->fullcmd[elem->i + 2] == ' ' || elem->fullcmd[elem->i + 2] == 0))
		elem->cmd[++elem->j] = 0;
	while (elem->fullcmd[elem->i] && elem->fullcmd[++elem->i] != '\'')
		elem->cmd[++elem->j] = elem->fullcmd[elem->i];
	if ((elem->i == (int)ft_strlen(elem->fullcmd)) && elem->fullcmd[elem->i] != '\'')
		return (quotes_error('\''));
	elem->i++;
	return (-2);
}

//echo "\$PATH"
int	double_quotes(t_parse *elem)
{
	int	ret;
	
	if (elem->i == ((int)ft_strlen(elem->fullcmd) - 1))
		return (quotes_error('\"'));
	while (elem->fullcmd[elem->i] && elem->fullcmd[++elem->i] != '"')
	{
		ret = 0;
		if (elem->fullcmd[elem->i] == '$' && elem->fullcmd[elem->i - 1] != '\\')
			ret = var_handler(elem, 0, 0, 0);
		if (elem->fullcmd[elem->i] == '\\')
		{	
			if (elem->fullcmd[elem->i + 1] == '$' || elem->fullcmd[elem->i + 1] == '\\' || elem->fullcmd[elem->i + 1] == '"')
				elem->i++;
		}
		if (ret != 1)
			elem->cmd[++elem->j] = elem->fullcmd[elem->i];
	}
	if (elem->fullcmd[elem->i] == '"' && (elem->fullcmd[elem->i + 1] == ' ' || elem->fullcmd[elem->i + 1] == '\0') && !elem->cmd[0])
		elem->cmd[0] = '\0';
	if ((elem->i == (int)ft_strlen(elem->fullcmd)) && elem->fullcmd[elem->i] != '"')
		return (quotes_error('\"'));
	elem->i++;
	if (elem->cmd[0] == '\0' && (elem->fullcmd[elem->i] == ' ' || elem->fullcmd[elem->i] == '\0'))
		return (1);
	return (-2);
}


int		quotes_handler(t_parse *elem)
{	
	int	ret;

	ret = -2;
	while (elem->fullcmd[elem->i] == '\'' || elem->fullcmd[elem->i] == '"')
	{
		while (elem->fullcmd[elem->i] == '"')
		{
			ret = double_quotes(elem);
			if (ret == -1)
				return (-1);
		}
		while (elem->fullcmd[elem->i] == '\'')
		{
			ret = single_quotes(elem);
			if (ret == -1)
				return (-1);
		}
	}
	if (elem->fullcmd[elem->i] == '\\')
		elem->i++;
	return (ret);
}

int		cmd_redir(t_parse *elem, int ret)
{
	if (elem->fullcmd[elem->i] && elem->fullcmd[elem->i] == '$' && (elem->i == 0 || elem->fullcmd[elem->i - 1] != '\\'))
	{
		if (elem->fullcmd[elem->i + 1] == '\\')
			elem->cmd[++elem->j] = elem->fullcmd[elem->i];
		else
			ret = var_handler(elem, 0, 0, 1);
	}
	if ((elem->fullcmd[elem->i] == '>' || elem->fullcmd[elem->i] == '<') && (elem->i == 0 || elem->fullcmd[elem->i - 1] != '\\'))
		ret = redir(elem, 0);
	return (ret);
}

char	*only_cmd(t_parse *elem)
{
	int ret;
	
	while (elem->fullcmd[elem->i] && elem->fullcmd[elem->i] != ' ')
	{
		ret = -2;
		if ((elem->fullcmd[elem->i] == '1' || elem->fullcmd[elem->i] == '2') && elem->fullcmd[elem->i + 1] == '>' && (!elem->cmd[0] || elem->fullcmd[elem->i - 1] == ' '))
			elem->i++;
		ret = quotes_handler(elem);
		if (ret == -1)
			return (NULL);
		ret = cmd_redir(elem, ret);
		if (ret == -1)
			return (NULL);
		if ((elem->fullcmd[elem->i] == ' ' && elem->fullcmd[elem->i - 1] != '\\') && (elem->cmd[0] || (!elem->cmd[0] && (elem->fullcmd[elem->i - 1] == '"' || elem->fullcmd[elem->i - 1] == '\'') && (elem->fullcmd[elem->i - 2] == '"' || elem->fullcmd[elem->i - 2] == '\'' || ret == 1))))
			break ;
		if (elem->i < (int)ft_strlen(elem->fullcmd) && ((elem->fullcmd[elem->i] == '$' && (elem->i == 0 || elem->fullcmd[elem->i - 1] == '\\')) || (elem->fullcmd[elem->i] != '$' && ret == -2)))
			elem->cmd[++elem->j] = elem->fullcmd[elem->i];
		if (elem->i < (int)ft_strlen(elem->fullcmd))
			elem->i++;
	}
	elem->cmd[elem->j + 1] = 0;
	return (elem->cmd);
}

int	init_parse_arg(t_parse *elem, int nb)
{
	if (nb < 0)
		return (FAILURE);
	elem->j = -1;
	elem->args[nb] = NULL;
	elem->args[nb] = (char *)malloc(sizeof(char) * (ft_strlen(elem->fullcmd) + 1));
	if (!elem->args[nb])
		return (FAILURE);
	elem->args[nb][0] = 0;
	while (elem->fullcmd[elem->i] && elem->fullcmd[elem->i] == ' ')
		elem->i++;
	elem->i--;
	return (SUCCESS);
}

int	inside_dquote(t_parse *elem, int nb, int err)
{
	if (elem->fullcmd[elem->i] == '$' && elem->fullcmd[elem->i - 1] != '\\')
	{
		err = var_handler(elem, 1, nb, 0);
		if (err == -2)
		{
			elem->i--;
			err = 1;
		}
	}
	if (elem->fullcmd[elem->i] == '\\')
	{
		if (elem->fullcmd[elem->i + 1] == '$' || elem->fullcmd[elem->i + 1] == '\\' || elem->fullcmd[elem->i + 1] == '"')
			elem->i++;
	}
	return (err);
}

int		double_quotes_arg(t_parse *elem, int nb)
{
	int err;
	
	if (elem->i == ((int)ft_strlen(elem->fullcmd) - 1))
		return (quotes_error('\"'));
	while (elem->fullcmd[elem->i] && elem->fullcmd[++elem->i] != '"')
	{
		err = 0;
		err = inside_dquote(elem, nb, err);
		if (err != 1)
			elem->args[nb][++elem->j] = elem->fullcmd[elem->i];
	}
	if (elem->fullcmd[elem->i] == '"' && (elem->fullcmd[elem->i + 1] == ' ' || elem->fullcmd[elem->i + 1] == '\0') && !elem->args[nb][0])
		elem->args[nb][0] = '\0';
	if ((elem->i == (int)ft_strlen(elem->fullcmd)) && elem->fullcmd[elem->i] != '"')
		return (quotes_error('\"'));
	elem->i++;
	if (elem->args[nb][0] == '\0' && (elem->fullcmd[elem->i] == ' ' || elem->fullcmd[elem->i] == '\0'))
		return (1);
	return (0);
}

int	single_quotes_arg(t_parse *elem, int nb)
{
	if (elem->i == ((int)ft_strlen(elem->fullcmd) - 1))
		return (quotes_error('\''));
	if (elem->fullcmd[elem->i + 1] == '\'' && (elem->fullcmd[elem->i + 2] == ' ' || elem->fullcmd[elem->i + 2] == 0))
		elem->args[nb][++elem->j] = 0;
	while (elem->fullcmd[elem->i] && elem->fullcmd[++elem->i] != '\'')
		elem->args[nb][++elem->j] = elem->fullcmd[elem->i];
	if ((elem->i == (int)ft_strlen(elem->fullcmd)) && elem->fullcmd[elem->i] != '\'')
		return (quotes_error('\''));
	elem->i++;
	return (1);
}

int	arg_quotes_handler(t_parse *elem, int nb, int err)
{
	if ((elem->fullcmd[elem->i] == '1' || elem->fullcmd[elem->i] == '2') && elem->fullcmd[elem->i + 1] == '>' && (elem->fullcmd[elem->i - 1] == ' '))
			elem->i++;
	while (elem->fullcmd[elem->i] == '\'' || elem->fullcmd[elem->i] == '"')
	{
		while (elem->fullcmd[elem->i] == '"')
		{
			err = double_quotes_arg(elem, nb);
			if (err == -1)
		 		return (-1);
		}
		while (elem->fullcmd[elem->i] == '\'')
		{
			err = single_quotes_arg(elem, nb);
			if (err == -1)
				return (-1);
		}
	}
	if (elem->fullcmd[elem->i] == '\\' && elem->i == (int)ft_strlen(elem->fullcmd) - 1 && elem->fullcmd[elem->i - 1] != '\\')
		return (err);
	if (elem->fullcmd[elem->i] == '\\')
		elem->i++;
	return (err);
}

int		arg_redir(t_parse *elem, int err, int nb)
{
	if (elem->fullcmd[elem->i] && elem->fullcmd[elem->i] == '$' && elem->fullcmd[elem->i - 1] != '\\')
	{
		if (elem->fullcmd[elem->i + 1] == '\\')
			elem->args[nb][++elem->j] = elem->fullcmd[elem->i];
		else
			err = var_handler(elem, 1, nb, 1);
	}
	if ((elem->fullcmd[elem->i] == '>' || elem->fullcmd[elem->i] == '<') && elem->fullcmd[elem->i - 1] != '\\')
		err = redir(elem, 0);
	return (err);
}

char	*parse_arg(t_parse *elem, int nb)
{
	int	err;
	
	if (init_parse_arg(elem, nb) == FAILURE)
		return (NULL);
	while (elem->fullcmd[elem->i] && (elem->fullcmd[++elem->i] != ' ' || (elem->fullcmd[elem->i] == ' ' && elem->fullcmd[elem->i - 1] == '\\')))
	{
		err = 0;
		err = arg_quotes_handler(elem, nb, err);
		if (err == -1)
			return (NULL);
		err = arg_redir(elem, err, nb);
		if (err == -3)
		{
			elem->args[nb][elem->j + 1] = 0;
			return (elem->args[nb]);
		}
		if ((elem->fullcmd[elem->i] == ' ' && elem->fullcmd[elem->i - 1] != '\\') && (elem->args[nb][0] || (!elem->args[nb][0] && (elem->fullcmd[elem->i - 1] == '"' || elem->fullcmd[elem->i - 1] == '\'') && (elem->fullcmd[elem->i - 2] == '"' || elem->fullcmd[elem->i - 2] == '\'' || err == 1))))
			break ;
		if (elem->fullcmd[elem->i] && (elem->fullcmd[elem->i] != ' ' || (elem->fullcmd[elem->i] == ' ' && elem->fullcmd[elem->i - 1] == '\\')) && /*err != 1 &&*/ err != 4 && ((elem->fullcmd[elem->i] == '$' && elem->fullcmd[elem->i - 1] != '$' && elem->fullcmd[elem->i + 1] == 0) || (elem->fullcmd[elem->i] == '$' && elem->fullcmd[elem->i - 1] == '\\') || (elem->fullcmd[elem->i] != '$')))
			elem->args[nb][++elem->j] = elem->fullcmd[elem->i];
	}
	elem->args[nb][++elem->j] = 0;
	return (elem->args[nb]);
}

int	init_args(t_parse *elem, int nb, int j)
{
	if (j == 1)
	{
		elem->arg = NULL;
		elem->tmp = NULL;
		elem->args[0] = ft_strdup(elem->cmd);
		if (!elem->args[0])
			return (FAILURE);
	}
	if (j == 2)
	{
		elem->tmp = elem->args;
		elem->args = (char **)malloc(sizeof(char *) * (nb + 2));
		if (!elem->args)
			return (FAILURE);
	}
	return (SUCCESS);
}

int	free_tmp(t_parse *elem, int i)
{
	while (i)
	{
		elem->args[i - 1] = ft_strdup(elem->tmp[i - 1]);
		if (!elem->args[i - 1])
			return (FAILURE);
		if (elem->tmp[i - 1])
			free(elem->tmp[i - 1]);
		i--;
	}
	return (SUCCESS);
}

int	arg_is_blank(char *arg, t_parse *elem)
{
	int	i;

	i = elem->i - 1;
	if (!arg[0] && i >= 2 && (elem->fullcmd[elem->i - 1] == '"' || elem->fullcmd[elem->i - 1] == '\'') && (elem->fullcmd[elem->i - 2] == '"' || elem->fullcmd[elem->i - 2] == '\'') && !elem->fullcmd[elem->i])
	{
		while (i >= 2 && (elem->fullcmd[i] == '"' || elem->fullcmd[i] == '\''))
		{
			if (elem->fullcmd[i - 1] != elem->fullcmd[i])
				return (0);
			i = i - 2;
		}
		if (elem->fullcmd[i] != ' ')
			return (0);
		return (1);
	}
	return (0);
}

int	form_args(t_parse *elem)
{
	int	i;
	
	init_args(elem, 0, 1);
	i = 1;
	while (1)
	{
		if (init_args(elem, i, 2) == FAILURE || free_tmp(elem, i) == FAILURE)
			return (FAILURE);
		if (elem->tmp)
			free(elem->tmp);
		elem->arg = parse_arg(elem, i);
		if (elem->arg == NULL)
			return (FAILURE);
		if (arg_is_blank(elem->arg, elem))
			elem->arg = parse_arg(elem, ++i);
		if (!elem->arg || (!elem->arg[0] && !elem->fullcmd[elem->i]))
		{
			free(elem->arg);
			break ;
		}
		i++;
	}
	elem->args[i] = NULL;
	return (SUCCESS);
}

int	add_tab_to_gb(t_parse *elem, char **args)
{
	int	k;

	if (add_address(&elem->p_data->collector, args) == -1)
			return (-1);
	k = 0;
	while (args[k] != NULL)
	{
		if (add_address(&elem->p_data->collector, args[k]) == -1)
			return (-1);
		k++;
	}
	return (SUCCESS);
}

int	parse(t_parse *elem)
{
	elem->redir.sstdin = 0;
	elem->redir.sstdout = 1;
	elem->redir.sstderr = 2;
	elem->redir.out1 = NULL;
	elem->redir.out2 = NULL;
	elem->redir.in = NULL;
	elem->redir.hd = NULL;
	elem->cmd = malloc(sizeof(char) * (ft_strlen(elem->fullcmd) + 1));
	if (!elem->cmd || add_address(&elem->p_data->collector, elem->cmd) == -1)
		return (FAILURE);
	elem->cmd[0] = 0;
	elem->i = 0;
	while (elem->fullcmd[elem->i] && contains(elem->fullcmd[elem->i], " \t\n\r\v\f"))
		elem->i++;
	elem->j = -1;
	elem->cmd = only_cmd(elem);
	if (!elem->cmd)
		return (FAILURE);
	elem->args = malloc(sizeof(char *) * 1);
	if (!elem->args || form_args(elem) == FAILURE || add_tab_to_gb(elem, elem->args) == -1)
		return (FAILURE);
	return (SUCCESS);
}
