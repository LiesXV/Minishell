/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 17:36:06 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/07/27 01:47:32 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_builtin(char *cmd, t_data *data)
{
	if (!ft_strncmp("exit", cmd, 4) && ft_strlen(cmd) == 4)
		return (built_exit(data, (*data->cmd_lst)->args), SUCCESS);
	if (!ft_strncmp("echo", cmd, 4) && ft_strlen(cmd) == 4)
		return (built_echo(data, (*data->cmd_lst)->args));
	if (!ft_strncmp("pwd", cmd, 3) && ft_strlen(cmd) == 3)
		return (built_pwd(data));
	if (!ft_strncmp("cd", cmd, 2) && ft_strlen(cmd) == 2)
		return (built_cd((*data->cmd_lst)->args));
	if (!ft_strncmp("env", cmd, 3) && ft_strlen(cmd) == 3)
		return (built_env(data));
	if (!ft_strncmp("export", cmd, 6) && ft_strlen(cmd) == 6)
		return (built_export(data));
	return (FAILURE);
}
