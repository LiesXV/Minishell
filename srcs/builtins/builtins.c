/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 17:36:06 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/07/18 10:11:39 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_builtin(char *cmd, t_data *data)
{
	if (!ft_strncmp("exit", cmd, 4))
		return (built_exit(data, (*data->cmd_lst)->args), SUCCESS);
	if (!ft_strncmp("echo", cmd, 4))
		return (built_echo((*data->cmd_lst)->args));
	if (!ft_strncmp("pwd", cmd, 3))
		return (built_pwd());
	if (!ft_strncmp("cd", cmd, 2))
		return (built_cd((*data->cmd_lst)->args));
	return (FAILURE);
}
