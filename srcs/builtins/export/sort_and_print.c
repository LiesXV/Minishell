/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_and_print.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 09:45:52 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/07/27 01:49:45 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	ft_lstsize(t_env *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

void	lstcpy(t_env **dst, t_env *lst)
{
	if ((*dst) == NULL)
	{
		*dst = malloc(sizeof(t_env));
		if (!(*dst))
			return ;
		(*dst)->var_name = ft_strdup(lst->var_name);
		(*dst)->var_content = ft_strdup(lst->var_content);
		(*dst)->next = NULL;
	}
}

t_env	*ft_lstgo_at(t_env *lst, int at)
{
	int	i;

	i = 0;
	while (lst && i < at)
	{
		if (!lst->next)
			return (lst);
		lst = lst->next;
		i++;
	}
	return (lst);
}

void	ft_lstadd_at(t_env **lst, t_env *new, int at)
{
	t_env	*cur;
	t_env	*tmp;

	if (lst && *lst)
	{
		cur = *lst;
		cur = ft_lstgo_at(cur, at + 1);
		tmp = cur->next;
		cur->next = new;
		cur->next->next = tmp;
		printf("yoyo");
	}
	else if (lst)
		*lst = new;
}

void	sort_env(t_env **sorted_env, t_env *env, int std)
{
	t_env	*tmp;
	t_env	*lst;
	int		i;

	lstcpy(sorted_env, env);
	env = env->next;
	lst = env;
	tmp = (*sorted_env);
	while (tmp)
	{
		i = 0;
		while (tmp && ft_strcmp(lst->var_name, tmp->var_name) >= 0)
		{
			if (ft_strcmp(lst->var_name, tmp->var_name) == 0)
			{
				tmp = tmp->next;
				i++;
			}
			else 
			{
				// printf("%s > %s\n", lst->var_name, tmp->var_name);
				tmp = tmp->next;
				i++;
			}
		}
		// printf("%s < %s\n", lst->var_name, tmp->var_name);
		get_next_line(0);
		print_env((*sorted_env), std);
		ft_lstadd_at(&tmp, lst, i);
		lst = lst->next;
		tmp = tmp->next;
	}
}

void	print_export(t_env *env)
{
	while (env)
	{
		printf("declare -x %s=%s\n", env->var_name, env->var_content);
		env = env->next;
	}
	return ;
}

void	sort_and_print(t_data *data)
{
	t_env	*sorted_env;

	sorted_env = malloc(sizeof(t_env));
	sorted_env = NULL;
	sort_env(&sorted_env, data->env, (*data->cmd_lst)->redir.sstdout);
	print_export(sorted_env);
}
