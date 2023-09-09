/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 19:33:29 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/09 17:22:45 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	free_all_env(t_env **env)
{
	t_env	*cur;
	t_env	*tmp;

	cur = *env;
	while (cur != NULL)
	{
		tmp = cur;
		cur = cur->next;
		if (tmp->var_name)
			free(tmp->var_name);
		if (tmp->var_content)
			free(tmp->var_content);
		free(tmp);
	}
	*env = NULL;
}

void	list_print_export(t_env *lst)
{
	t_env	*copy;

	copy = lst;
	while (copy)
	{
		if (copy->var_content)
			printf("declare -x %s=\"%s\"\n", copy->var_name, copy->var_content);
		else if (copy->var_name)
			printf("declare -x %s\n", copy->var_name);
		copy = copy->next;
	}
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
