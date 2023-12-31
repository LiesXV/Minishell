/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 11:39:21 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/27 15:22:40 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_env_val(t_data *data, char *name)
{
	t_env	*cur;
	char	*tmp;

	if (!name || name[0] == 0)
	{
		tmp = ft_strdup("");
		if (!tmp || add_address(&data->collector, tmp))
			return (NULL);
		return (tmp);
	}
	cur = data->env;
	while (cur)
	{
		if (!ft_strcmp(cur->var_name, name))
		{
			tmp = ft_strdup(cur->var_content);
			if (tmp && add_address(&data->collector, tmp))
				return (NULL);
			return (tmp);
		}
		cur = cur->next;
	}
	return (NULL);
}

t_env	*create_env(void)
{
	char	*pwd;
	char	*cwd;
	t_env	*env;

	pwd = ft_strdup("PWD=");
	if (!pwd)
		return (NULL);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (free(pwd), NULL);
	pwd = ft_strfjoin(pwd, cwd);
	if (!pwd)
		return (free(cwd), NULL);
	env = new_env(pwd);
	if (!pwd)
		return (free(pwd), free(cwd), NULL);
	ft_lstadd_back(&env, new_env("SHLVL=1"));
	ft_lstadd_back(&env, new_env("OLDPWD"));
	return (free(cwd), free(pwd), env);
}

t_env	*get_env(t_data *data)
{
	int		j;
	t_env	*env;
	t_env	*result;

	j = -1;
	env = NULL;
	result = env;
	if (data->envp[0] == NULL)
		result = create_env();
	else
	{
		while (data->envp[++j])
		{
			env = new_env(data->envp[j]);
			ft_lstadd_back(&result, env);
			env = env->next;
		}
	}
	return (result);
}

int	built_env(t_data *data, char **args)
{
	if (args[1] && args[1][0] != '|')
	{
		ft_putstr_fd("minishell: env does not support options or arguments\n", \
			2);
		return (SUCCESS);
	}
	print_env(data->env, (*data->cmd_lst)->redir.sstdout);
	return (SUCCESS);
}
