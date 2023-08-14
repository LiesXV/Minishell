/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 18:33:11 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/08/06 00:43:08 by ibenhaim         ###   ########.fr       */
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
	if (!new)
		return (NULL);
	add_address(&data->collector, new);
	new->cmd = ft_split_pipex(cmd, ' ');
	if (!new->cmd || !new->cmd[0])
		return (new_error(new, cmd));
	add_address(&data->collector, new->cmd);
	while (new->cmd[++i])
		add_address(&data->collector, new->cmd[i]);
	new->path = get_path(new->cmd[0], data);
	if (!new->path)
		return (new_error(new, cmd));
	add_address(&data->collector, new->path);
	new->next = NULL;
	return (new);
}
