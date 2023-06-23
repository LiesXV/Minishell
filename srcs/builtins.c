/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 17:36:06 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/06/20 15:27:21 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_builtin(char *cmd, t_data *data)
{
	char	**params;
	int		i;

	i = 0;
	params = ft_split(cmd, ' ');
	if (!params)
		return (-1);
	printf("cmd : %s\n", params[0]);
	while (params[++i])
		add_address(&data->collector, params[i]);
	add_address(&data->collector, params);
	if (!ft_strncmp("exit", params[0], 4))
		built_exit(params[1], data);
	return (FAILURE);
}
