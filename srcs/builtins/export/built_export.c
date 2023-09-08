/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 09:35:07 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/08 14:24:37 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	export_var(t_data *data);

int	built_export(t_data *data)
{
	if (((*data->cmd_lst)->args[1] == NULL) || \
	((*data->cmd_lst)->args[1][0] == '|'))
		sort_and_print(data->env);
	else
		export_var(data);
	return (SUCCESS);
}

void	swap_cont(t_env	*env, char *str)
{
	free(env->var_content);
	env->var_content = ft_strdup(str);
}

int	is_varname_good(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (i == 0) 
		{
			if ((str[i] >= '0' && str[i] <= '9') || str[i] == '$')
				return (FAILURE);
		}
		if ((str[i] < 'a' || str[i] > 'z') && (str[i] < 'A' || str[i] > 'Z') \
			&& (str[i] < '0' || str[i] > '9') && (str[i] != '_'))
			return (FAILURE);
		i++;
		if (str[i] && !str[i + 1] && str[i] == '+')
			return (2);
	}
	return (SUCCESS);
}

int	aggregate_cont(char **parse, t_env *envi)
{
	parse[0][ft_strlen(parse[0]) - 1] = '\0';
	while (envi)
	{
		if (!ft_strncmp(parse[0], envi->var_name, ft_strlen(envi->var_name)))
		{
			if (!parse[1])
				return (free_split(parse), 3);
			if (!envi->var_content)
				envi->var_content = ft_strdup(parse[1]);
			else
				envi->var_content = ft_strfjoin(envi->var_content, parse[1]);
			free_split(parse);
			return (3);
		}
		envi = envi->next;
	}
	free_split(parse);
	return (2);
}

int	search_env(t_data *data, char *env)
{
	char	**split;
	t_env	*envi;
	int		name;

	name = -1;
	envi = (*data).env;
	split = ft_split(env, '=');
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
		split[0]), FAILURE);
	}
	else if (name == 2)
		return (aggregate_cont(split, envi));
	while (envi)
	{
		if (!ft_strncmp(split[0], envi->var_name, ft_strlen(split[0]) + 1) \
			&& split[1])
			return (swap_cont(envi, env + ft_strlen(split[0]) + 1), free_split(split), FAILURE);
		envi = envi->next;
	}
	free_split(split);
	return (SUCCESS);
}

char	*if_nexistant_var(char *str)
{
	char	*newenv;
	int		i;
	int		j;

	i = 0;
	j = 0;
	newenv = malloc(sizeof(char) * ft_strlen(str));
	if (!newenv)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '+' && i == j)
			i++;
		newenv[j] = str[i];
		i++;
		j++;
	}
	newenv[j] = '\0';
	return (newenv);
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
		}
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