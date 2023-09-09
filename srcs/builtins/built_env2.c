/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_env2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 17:26:34 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/09 17:26:34 by ibenhaim         ###   ########.fr       */
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
	char	*name;

	name = ft_substr(line, 0, ft_strchri(line, '='));
	if (is_varname_good(name) == FAILURE)
		return (NULL);
	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->var_name = name;
	if (!(strchr(line, '=')))
		new->var_content = NULL;
	else
	{
		new->var_content = ft_strdup(ft_strchr(line, '=') + 1);
	}
	new->next = NULL;
	return (new);
}

void	print_env(t_env *env, int std)
{
	while (env)
	{
		if (env->var_content)
		{
			ft_putstr_fd(env->var_name, std);
			ft_putstr_fd("=", std);
			ft_putstr_fd(env->var_content, std);
			ft_putstr_fd("\n", std);
		}
		env = env->next;
	}
	return ;
}
