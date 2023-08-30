/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 18:11:03 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/08/30 14:58:29 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*join_cmd(char *path, char *cmd)
{
	char	*tmp;
	char	*result;

	tmp = ft_strdup(path);
	if (!tmp)
		return (NULL);
	tmp = ft_strfjoin(tmp, "/");
	if (!tmp)
		return (NULL);
	result = ft_strfjoin(tmp, cmd);
	if (!result)
		return (NULL);
	return (result);
}

char	*get_path(char *cmd, t_data *data)
{
	char	**result;
	char	*test;
	int		i;

	i = -1;
	test = ft_strdup(cmd);
	if (!test)
		return (NULL);
	if (cmd != NULL && ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (test);
		return (NULL);
	}
	result = ft_split(data->path, ':');
	if (!result)
		return (NULL);
	add_address(&data->collector, result);
	while (result[++i])
		add_address(&data->collector, result[i]);
	i = -1;
	while (result[++i])
	{
		free(test);
		test = join_cmd(result[i], cmd);
		if (!test)
			return (NULL);
		if (access(test, F_OK | X_OK) >= 0)
			return (add_address(&data->collector, test), test);
	}
	return (free(test), NULL);
}
