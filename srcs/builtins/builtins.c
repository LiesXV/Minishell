/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 17:36:06 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/26 13:05:43 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_builtin(char **cmd, t_data *data)
{
	if (!ft_strncmp("exit", cmd[0], 4) && ft_strlen(cmd[0]) == 4)
		return (built_exit(data, cmd), SUCCESS);
	if (!ft_strncmp("echo", cmd[0], 4) && ft_strlen(cmd[0]) == 4)
		return (built_echo(cmd));
	if (!ft_strncmp("pwd", cmd[0], 3) && ft_strlen(cmd[0]) == 3)
		return (built_pwd(data));
	if (!ft_strncmp("cd", cmd[0], 2) && ft_strlen(cmd[0]) == 2)
		return (built_cd((*data->cmd_lst)->args, data));
	if (!ft_strncmp("env", cmd[0], 3) && ft_strlen(cmd[0]) == 3)
		return (built_env(data));
	if (!ft_strncmp("export", cmd[0], 6) && ft_strlen(cmd[0]) == 6)
		return (built_export(data));
	if (!ft_strncmp("unset", cmd[0], 5) && ft_strlen(cmd[0]) == 5)
		return (built_unset(data, (*data->cmd_lst)->args));
	return (FAILURE);
}

char	*is_built(char *cmd)
{
	if (!ft_strncmp("exit", cmd, 4) && ft_strlen(cmd) == 4)
		return ("exit");
	if (!ft_strncmp("echo", cmd, 4) && ft_strlen(cmd) == 4)
		return ("echo");
	if (!ft_strncmp("pwd", cmd, 3) && ft_strlen(cmd) == 3)
		return ("pwd");
	if (!ft_strncmp("cd", cmd, 2) && ft_strlen(cmd) == 2)
		return ("cd");
	if (!ft_strncmp("env", cmd, 3) && ft_strlen(cmd) == 3)
		return ("env");
	if (!ft_strncmp("export", cmd, 6) && ft_strlen(cmd) == 6)
		return ("export");
	if (!ft_strncmp("unset", cmd, 5) && ft_strlen(cmd) == 5)
		return ("unset");
	return (NULL);
}
