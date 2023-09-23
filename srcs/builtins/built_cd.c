/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 16:02:45 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/23 10:07:42 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	change_oldpwd(t_env *cur, char *tmp)
{
	while (cur)
	{
		if (ft_strncmp(cur->var_name, "OLDPWD\0", 7) == 0)
		{
			if (cur->var_content)
				free(cur->var_content);
			cur->var_content = ft_strdup(tmp);
			free(tmp);
			if (!cur->var_content)
				return (FAILURE);
			return (SUCCESS);
		}
		cur = cur->next;
	}
	return (FAILURE);
}

int	change_pwd(t_data *data)
{
	t_env	*cur;
	t_env	*cpy;
	char	*tmp;

	tmp = NULL;
	cpy = data->env;
	cur = data->env;
	while (cur)
	{
		if (ft_strncmp(cur->var_name, "PWD\0", 4) == 0)
		{
			tmp = ft_strdup(cur->var_content);
			if (!tmp)
				return (FAILURE);
			free(cur->var_content);
			cur->var_content = getcwd(NULL, 0);
			if (!cur->var_content)
				return (free(tmp), FAILURE);
			break ;
		}
		cur = cur->next;
	}
	if (change_oldpwd(cpy, tmp) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

int	built_cd(char **args, t_data *data)
{
	int	argc;

	argc = 0;
	while (args[argc] && args[argc][0] != '|')
		argc++;
	if (argc == 1)
	{
		if (chdir(getenv("HOME")) != 0 || change_pwd(data) < 0)
		{
			perror("cd");
			return (SUCCESS);
		}
	}
	else if (argc == 2)
	{
		if (chdir(args[1]) != 0 || change_pwd(data) < 0)
		{
			perror("cd");
			return (SUCCESS);
		}
		g_end_status = 0;
	}
	else
		ft_putstr_fd("cd: too much arguments\n", 2);
	return (SUCCESS);
}
