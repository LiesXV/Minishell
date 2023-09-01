/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_and_print.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 09:45:52 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/01 12:28:36 by ibenhaim         ###   ########.fr       */
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
			printf("declare -x %s=' '\n", copy->var_name);
		copy = copy->next;
	}
}

static void	swap_nodes(t_env *current_node, t_env *next_node)
{
	char	*var_name;
	char	*var_content;

	var_name = NULL;
	var_content = NULL;
	var_name = current_node->var_name;
	current_node->var_name = next_node->var_name;
	next_node->var_name = var_name;
	var_content = current_node->var_content;
	current_node->var_content = next_node->var_content;
	next_node->var_content = var_content;
}

static void	sort_in_alphabetic_order(t_env *head)
{
	int		to_sort;
	t_env	*tmp;
	t_env	*end;

	if (head == NULL)
		return ;
	to_sort = 1;
	end = NULL;
	while (to_sort)
	{
		to_sort = 0;
		tmp = head;
		while (tmp->next != end)
		{
			if (ft_strncmp(tmp->var_name, tmp->next->var_name, \
				ft_strlen(tmp->var_name)) > 0)
			{
				swap_nodes(tmp, tmp->next);
				to_sort = 1;
			}
			tmp = tmp->next;
		}
		end = tmp;
	}
}

t_env	*new_env_export(char *var_name, char *var_content)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->var_name = ft_strdup(var_name);
	if (var_content)
		new->var_content = ft_strdup(var_content);
	else
		new->var_content = NULL;
	new->next = NULL;
	return (new);
}

static t_env	*lstcpy(t_env *lst)
{
	t_env	*node;
	t_env	*copy_env;

	node = NULL;
	copy_env = NULL;
	if (!lst)
		return (NULL);
	while (lst != NULL)
	{
		node = new_env_export(lst->var_name, lst->var_content);
		if (!node)
			return (NULL);
		ft_lstadd_back(&copy_env, node);
		lst = lst->next;
	}
	return (copy_env);
}

int	sort_and_print(t_env *lst)
{
	t_env	*copy_env;
	t_env	*cpy_cpy;

	copy_env = NULL;
	copy_env = lstcpy(lst);
	if (!copy_env)
		return (FAILURE);
	cpy_cpy = copy_env;
	sort_in_alphabetic_order(copy_env);
	list_print_export(copy_env);
	free_all_env(&cpy_cpy);
	return (SUCCESS);
}
