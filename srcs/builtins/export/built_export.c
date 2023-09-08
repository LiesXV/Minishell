/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 09:35:07 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/08 20:08:54 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	swap_cont(t_env	*env, char *str)
{
	free(env->var_content);
	env->var_content = ft_strdup(str);
}

int	parse_search_env(char **split, t_env *envi, char *env)
{
	int	name;

	name = -1;
	if (!split[0])
	{
		g_end_status = 1;
		return (printf("minishell: export: '%s': not a valid identifier\n", env) \
			, FAILURE);
	}
	name = is_varname_good(split[0]);
	if (name == FAILURE)
	{
		g_end_status = 1;
		return (printf("minishell: export: '%s': not a valid identifier\n", \
		split[0]), free_split(split), FAILURE);
	}
	else if (name == 2)
		return (aggregate_cont(split, envi));
	return (SUCCESS);
}

int	search_env(t_data *data, char *env)
{
	char	**split;
	t_env	*envi;
	int		test;

	test = 0;
	envi = (*data).env;
	split = ft_split(env, '=');
	test = parse_search_env(split, envi, env);
	if (test != SUCCESS)
		return (test);
	while (envi)
	{
		if (!ft_strncmp(split[0], envi->var_name, ft_strlen(split[0]) + 1) \
			&& split[1])
			return (swap_cont(envi, env + ft_strlen(split[0]) + 1), \
				free_split(split), FAILURE);
		envi = envi->next;
	}
	free_split(split);
	return (SUCCESS);
}

int	export_var(t_data *data)
{
	char	**parse;
	int		i;
	int		test;

	i = 0;
	test = -1;
	parse = (*data->cmd_lst)->args;
	while (parse[++i])
	{
		test = search_env(data, parse[i]);
		if (test == SUCCESS)
			ft_lstadd_back(&data->env, new_env(parse[i]));
		if (test == 2)
		{
			parse[i] = if_nexistant_var(parse[i]);
			if (add_address(&data->collector, parse[i]) == 1)
				return (FAILURE);
			ft_lstadd_back(&data->env, new_env(parse[i]));
		}
	}
	return (1);
}

int	built_export(t_data *data)
{
	if (((*data->cmd_lst)->args[1] == NULL) || \
	((*data->cmd_lst)->args[1][0] == '|'))
		sort_and_print(data->env);
	else
		export_var(data);
	return (SUCCESS);
}
