/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 17:36:06 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/06/24 00:12:00 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_builtin(char *cmd, t_data *data)
{
	if (!ft_strncmp("exit", cmd, 4))
		built_exit(data, (*data->cmd_lst)->args);
	return (FAILURE);
}
