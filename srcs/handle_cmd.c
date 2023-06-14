/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 18:38:00 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/06/14 19:02:45 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../includes/minishell.h"

// int	handle_cmd(t_data *data)
// {
// 	char		*result;
// 	char		*cmd;
// 	t_list		*lst;	

// 	result = ft_split(data->path, ':');
// 	if (!result)
// 		return (-1);
// 	add_address(&data.collector, result);
// 	cmd = get_path(result, input, data);
// 	if (!cmd)
// 	{
// 		free_all(&data.collector);
// 		return (FAILURE);
// 	}
// 	else
// 	{
// 		add_address(&data.collector, cmd);
// 		lst = ft_lstnew(cmd, data);
// 	}
// }
