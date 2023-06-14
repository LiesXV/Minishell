/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 18:33:11 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/06/14 19:23:22 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_list	*new_error(t_list *new, char *cmd)
{
	printf("%s\n", new->path);
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

	new = malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->cmd = ft_split_pipex(cmd, ' ');
	if (!new->cmd || !new->cmd[0])
		return (new_error(new, cmd));
	new->path = get_path(new->cmd[0], data);
	if (!new->path)
		return (new_error(new, cmd));
	new->next = NULL;
	return (new);
}
