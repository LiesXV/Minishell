/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 18:33:11 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/03 01:09:59 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_list	*new_error(t_list *new, char *cmd)
{
	printf("command not found: %s\n", cmd);
	if (new->cmd)
		free_split(new->cmd);
	if (new)
		free(new);
	return (NULL);
}

t_list	*add_cmd(char *cmd, t_data *data)
{
	t_list	*new;
	int		i;

	i = -1;
	new = malloc(sizeof(t_list));
	if (!new || add_address(&data->collector, new) == 1)
		return (NULL);
	new->cmd = ft_split_pipex(cmd, ' ');
	if (!new->cmd || !new->cmd[0] || add_address(&data->collector, new->cmd) == 1)
		return (new_error(new, cmd));
	while (new->cmd[++i])
	{
		if (add_address(&data->collector, new->cmd[i]) == 1)
			return (new_error(new, cmd));
	}
	new->path = get_path(new->cmd[0], data);
	if (!new->path || add_address(&data->collector, new->path) == 1)
		return (new_error(new, cmd));
	new->next = NULL;
	return (new);
}
