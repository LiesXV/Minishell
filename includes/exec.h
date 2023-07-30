/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:51:27 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/07/30 01:18:19 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H
# include "./minishell.h"

void	handle_exec(t_data *data);
char	*get_path(char *cmd, t_data *data);
int		get_cmd(t_parse *lst, t_data *data);
t_list	*add_cmd(char *cmd, t_data *data);

#endif