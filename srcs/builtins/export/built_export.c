/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 09:35:07 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/08/14 13:29:22 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	export_var(t_data *data);

int	built_export(t_data *data)
{
	if (((*data->cmd_lst)->args[1] == NULL) || ((*data->cmd_lst)->args[1][0] == '|'))
		sort_and_print(data->env);
	else
		export_var(data);
	return (SUCCESS);
}

void	swap_cont(t_env	*env, char *str)
{
	free(env->var_content);
	env->var_content = ft_strdup(str);
	printf("new value of %s is %s\n", env->var_name, env->var_content);
}

int	is_alpha(char c)
{
	if (c >= 'a' || c <= 'z')
		return (SUCCESS);
	if (c >= 'A' || c <= 'Z')
		return (SUCCESS);
	return (FAILURE);
}

int	search_env(t_data *data, char *env)
{
	char	**split;
	t_env	*envi;

	envi = (*data).env;
	split = ft_split(env, '=');
	if (!split)
		return (FAILURE);
	if (is_alpha(split[0][0]) == FAILURE)
		return (printf("minishell: export: '%s': not a valid identifier\n", split[0]), FAILURE);
	while (envi)
	{
		if (!ft_strcmp(split[0], envi->var_name))
			return (swap_cont(envi, env + ft_strlen(split[0]) + 1), FAILURE);
		envi = envi->next;
	}
	return (SUCCESS);
}

int	export_var(t_data *data)
{
	char	**parse;
	int		i;

	i = 0;
	parse = (*data->cmd_lst)->args;
	while (parse[++i])
	{
		if (!search_env(data, parse[i]))
			ft_lstadd_back(&data->env, new_env(parse[i]));
	}
	return (1);
}

/* 
RESTE A FAIRE

parser correctement le export parce que la cest vraiment dla zeub
empecher d'export 2 fois le meme nom ou remplacer la valeur
s'occuper des leaks et touuuuut et tout
faire le unset
*/