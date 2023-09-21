/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   formating.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 16:01:54 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/21 19:00:55 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	add_space_before(char *tmp, char *fullcmd, int count, char **new)
{
	int		i;
	char	*copy;

	i = 0;
	copy = NULL;
	copy = ft_strdup(*new);
	count--;
	if (tmp[i] == ' ')
	{
		count = add_space_before_util(fullcmd, count, tmp);
		if (add(fullcmd, copy, new, count) == -1)
			return (-1);
	}
	if (copy)
		free(copy);
	return (1);
}

void	add_space_after(char *tmp, char *fullcmd, int count, char **new)
{
	int	i;
	int	j;

	j = ft_strlen(*new);
	if (!tmp)
		return ;
	i = ft_strlen(tmp) - 1;
	if (i < 0)
		return ;
	if (tmp[i] == ' ' && after_var(count, fullcmd) && fullcmd[count] == '$')
		return ;
	if (tmp[i] == ' ' && contains(fullcmd[count], "\"'/@,$[]\\"))
	{
		(*new)[j] = ' ';
		(*new)[j + 1] = '\0';
	}
}

int	init_cmds(t_data *data, t_parse *new, char *str)
{
	new->p_data = data;
	new->fullcmd = str;
	new->piplist = NULL;
	new->args = NULL;
	new->next = NULL;
	if (parse(new) == FAILURE)
		return (1);
	if (contains('|', new->fullcmd))
	{
		if (handle_pipes(new) == FAILURE)
			return (1);
	}
	else
		new->piplist = NULL;
	return (0);
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
	while (cmds[i] != NULL)
	{
		new = malloc(sizeof(t_parse));
		if (!new || add_address(&data->collector, new) == 1)
			return (NULL);
		if (init_cmds(data, new, cmds[i]) == 1)
			return (NULL);
		parse_add_back(head, new);
		i++;
	}
	return (head);
}
