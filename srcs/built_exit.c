/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 17:39:38 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/06/24 00:12:32 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	built_exit(t_data *data, char **args)
{
	if (args[1])
		exit(ft_atoi(args[1]));
	else
		exit(0);
	free_all(&data->collector);
}
