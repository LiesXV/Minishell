/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 23:37:18 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/08/01 17:35:09 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void    handle_exec(t_data *data)
{
	t_parse	*cur;

	cur = *data->cmd_lst;
	if (cur->piplist)
		pipex(data);
		// printlist(&cur);
	else if (is_builtin(cur->cmd, data) == FAILURE)
	{
		cur->path = get_path(cur->cmd, data);
		if (cur->cmd && !only_spaces(cur->cmd))
			get_cmd(cur, data);
		wait(NULL);
	}
}
