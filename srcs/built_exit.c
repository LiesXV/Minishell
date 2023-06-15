/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 17:39:38 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/06/15 18:53:29 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	built_exit(char *param, t_data *data)
{
	printf("ntm batard excite toi\n");
	if (param)
		exit(ft_atoi(param));
	else
		exit(0);
	free_all(&data->collector);
}
