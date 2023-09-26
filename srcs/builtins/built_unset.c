/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_unset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 11:48:02 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/26 10:59:08 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	find_and_remove(t_env **env, char *var_name)
{
	t_env	*cur;
	t_env	*prev;

	cur = *env;
	prev = NULL;
	while (cur)
	{
		if (ft_strncmp(cur->var_name, var_name, ft_strlen(cur->var_name)) == 0)
		{
			if (prev == NULL)
				*env = cur->next;
			else
				prev->next = cur->next;
			free(cur->var_name);
			free(cur->var_content);
			free(cur);
			return (SUCCESS);
		}
		prev = cur;
		cur = cur->next;
	}
	return (FAILURE);
}

int	built_unset(t_data *data, char	**args)
{
	int	i;

	i = 0;
	if (!args[1])
		return (SUCCESS);
	while (args[++i] && args[i][0] != '|')
		find_and_remove(&data->env, (*data->cmd_lst)->args[1]);
	return (SUCCESS);
}
