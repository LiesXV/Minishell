/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 11:39:21 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/07/26 12:20:12 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	*ft_lstlast(t_env *lst)
{
	while (lst)
	{
		if (!lst->next)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}

void	ft_lstadd_back(t_env **lst, t_env *new)
{
	t_env	*cur;

	if (lst && *lst)
	{
		cur = *lst;
		cur = ft_lstlast(cur);
		cur->next = new;
	}
	else if (lst)
		*lst = new;
}

int	ft_strchri(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
			return (i);
		i++;
	}
	if (s[i] == (char)c)
		return (i);
	return (-1);
}

t_env	*new_env(char *line)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->var_name = ft_substr(line, 0, ft_strchri(line, '='));
	new->var_content = ft_strdup(ft_strchr(line, '=') + 1);
	new->next = NULL;
	return (new);
}
void	print_env(t_env *env)
{
	while (env)
	{
		printf("[%s][=][%s]\n", env->var_name, env->var_content);
		env = env->next;
	}
	return ;
}


t_env	*get_env(t_data *data)
{
	int		j;
	t_env	*env = NULL;
	t_env	*result;

	j = -1;
	result = env;
	while (data->envp[++j])
	{
		env = new_env(data->envp[j]);
		ft_lstadd_back(&result, env);
		env = env->next;
	}
	return (result);
}


int	built_env(t_data *data)
{
	print_env(data->env);
	return (0);
}
