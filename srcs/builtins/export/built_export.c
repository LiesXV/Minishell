/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 09:35:07 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/08/03 21:08:09 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	built_export(t_data *data)
{
	if ((*data->cmd_lst)->args[1] == NULL)
		sort_and_print(data->env);
	else
		ft_lstadd_back(&data->env, new_env((*data->cmd_lst)->fullcmd + 7));
	return (SUCCESS);
}

/* 
	RESTE A FAIRE

parser correctement le export parce que la cest vraiment dla zeub
empecher d'export 2 fois le meme nom ou remplacer la valeur
s'occuper des leaks et touuuuut et tout
faire le unset

*/