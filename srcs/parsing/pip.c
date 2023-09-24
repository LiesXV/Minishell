/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pip.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 19:45:13 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/23 18:14:59 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**args_to_pip(t_parse *elem)
{
	char	**pip;
	int		k;

	k = 0;
	while (elem->args[k])
		k++;
	pip = malloc(sizeof(char *) * k + 1);
	if (!pip)
		return (NULL);
	k = 0;
	while (elem->args[++elem->j] && elem->args[elem->j] 
		&& elem->args[elem->j][0] != '|')
	{
		pip[k] = elem->args[elem->j];
		k++;
	}
	pip[k] = NULL;
	return (pip);
}

char	**pip_hd(t_parse *elem, char **src, char *line, char **dest)
{
	int			i;
	int			nb;
	static int	filled = 0;

	if (dest == NULL)
		dest = malloc(sizeof(char *) * ft_strlen(elem->fullcmd) + 1);
	if (!dest || add_address(&elem->p_data->collector, dest) == 1 || !src)
		return (NULL);
	nb = 0;
	while (src[nb])
		nb++;
	i = 0;
	while (filled < nb && src[filled] 
		&& ft_strnstr(line, "<<", ft_strlen(line)) 
		&& ft_strnstr(line, src[filled], ft_strlen(line)))
	{
		dest[i] = src[filled];
		filled++;
		i++;
	}
	if (i == 0)
		return (NULL);
	else
		dest[i] = NULL;
	return (dest);
}

char	**init_piplist(t_parse *elem)
{
	char	**strs;

	elem->piplist = malloc(sizeof(t_piplist *));
	if (!elem->piplist 
		|| add_address(&elem->p_data->collector, elem->piplist) == 1)
		return (NULL);
	*elem->piplist = NULL;
	strs = NULL;
	strs = trixsplit(elem->fullcmd, '|');
	if (!strs)
		return (NULL);
	elem->j = -1;
	return (strs);
}

int	handle_pipes_utils(t_parse *elem, t_piplist *new, char	*str)
{
	if (new->cmd[0])
	{
		if (is_built(new->cmd[0]) != NULL)
			new->path = ft_strdup(is_built(new->cmd[0]));
		else
			new->path = get_path(new->cmd[0], elem->p_data);
		if (add_address(&elem->p_data->collector, new->path) == 1)
			return (FAILURE);
	}
	new->redir = create_pip_redir(str, elem);
	new->redir.hd = pip_hd(elem, elem->redir.hd, str, new->redir.hd);
	new->next = NULL;
	return (SUCCESS);
}

int	handle_pipes(t_parse *elem)
{
	char		**strs;
	int			i;
	t_piplist	*new;

	if (valid_pip('|', elem->fullcmd) > -1)
	{
		strs = init_piplist(elem);
		if (!strs)
			return (FAILURE);
		i = -1;
		while (strs[++i])
		{
			new = malloc(sizeof(t_piplist));
			if (!new || add_address(&elem->p_data->collector, new) == 1)
				return (free_tab((void **)strs), free(strs), FAILURE);
			new->cmd = args_to_pip(elem);
			if (!new->cmd 
				|| add_address(&elem->p_data->collector, new->cmd) == 1 
				|| handle_pipes_utils(elem, new, strs[i]) == FAILURE)
				return (free_tab((void **)strs), free(strs), FAILURE);
			pip_add_back(elem->piplist, new);
		}
		return (free_tab((void **)strs), free(strs), SUCCESS);
	}
	return (-1);
}
