/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 18:11:03 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/03 01:12:03 by lmorel           ###   ########.fr       */
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
	if (!result || add_address(&data->collector, result) == 1)
		return (NULL);
	while (result[++i])
	{
		if (add_address(&data->collector, result[i]) == 1)
			return (NULL);
	}
	i = -1;
	while (result[++i])
	{
		free(test);
		test = join_cmd(result[i], cmd);
		if (!test)
			return (NULL);
		if (access(test, F_OK | X_OK) >= 0)
			return (test);
	}
	return (free(test), NULL);
}
