/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 11:39:21 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/07/20 14:28:53 by ibenhaim         ###   ########.fr       */
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

t_env	*new_env(char *line)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->var_name = ft_substr(line, 0, ft_strlen(ft_strchr(line, '=') - 1));
	new->var_content = ft_strdup(ft_strchr(line, '=') + 1);
	// printf("%s=%s\n", new->var_name, new->var_content);
	new->next = NULL;
	return (new);
}
void	print_env(t_env *env)
{
	while (env)
	{
		printf("%s=%s\n", env->var_name, env->var_content);
		// printf("yooy\n");
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
