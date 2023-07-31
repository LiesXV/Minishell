/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 09:35:07 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/07/31 12:20:44 by ibenhaim         ###   ########.fr       */
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
n̶e̶ p̶a̶s̶ f̶a̶i̶r̶e̶ s̶'̶a̶f̶f̶i̶c̶h̶e̶r̶ a̶v̶e̶c̶ "̶e̶n̶v̶"̶ l̶e̶s̶ v̶a̶r̶i̶a̶b̶l̶e̶s̶ s̶a̶n̶s̶ v̶a̶l̶e̶u̶r̶

*/