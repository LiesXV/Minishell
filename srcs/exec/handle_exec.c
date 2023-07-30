/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 23:37:18 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/07/30 02:16:00 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void    handle_exec(t_data *data)
{
	t_parse	*cur;

	cur = *data->cmd_lst;
	if (cur->piplist)
	{
		printf("minishell: pipe handling not available yet\n");
	}
	else if (is_builtin(cur->cmd, data) == FAILURE)
	{
		cur->path = get_path(cur->cmd, data);
		get_cmd(cur, data);
		wait(NULL);
	}
}
