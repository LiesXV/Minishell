/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 12:17:25 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/06/24 12:17:25 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	built_echo(t_data *data, char **args)
{
	int	i;

	i = 1;
	if (args[1] && !ft_strncmp(args[1], "-n", 2) && ft_strlen(args[1]) == 2)
		i++;
	while (args[i])
	{
		if (ft_putstr_fd(args[i], (*data->cmd_lst)->redir.sstdout) != (int)ft_strlen(args[i]))
			return (ft_putstr_fd("write error", 2), FAILURE);
		if (args[i + 1])
			ft_putchar_fd(' ', (*data->cmd_lst)->redir.sstdout);
		i++;
	}
	if (args[1] && !(!ft_strncmp(args[1], "-n", 2) && ft_strlen(args[1]) == 2))
		ft_putchar_fd('\n', (*data->cmd_lst)->redir.sstdout);
	return (SUCCESS);
}
