/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_replacing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 19:34:47 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/27 16:37:42 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*replace_in_cmdarg(t_parse *elem, char *var, char *str)
{
	char	*tmp;
	int		i;

	i = -1;
	tmp = NULL;
	str[elem->j + 1] = 0;
	tmp = ft_strjoin(str, var);
	if (!tmp)
		return (NULL);
	elem->j = -1;
	str = malloc(sizeof(char)
			* (ft_strlen(tmp) + ft_strlen(elem->fullcmd) + 1));
	if (!str || add_address(&elem->p_data->collector, str))
		return (NULL);
	while (tmp[++i])
		str[++elem->j] = tmp[i];
	if (tmp)
		free(tmp);
	return (str);
}

char	*replace_in_redir(t_parse *elem, char *var, char *str)
{
	char	*tmp;
	int		i;

	i = -1;
	tmp = NULL;
	str[elem->redir.i + 1] = 0;
	tmp = ft_strjoin(str, var);
	if (!tmp)
		return (NULL);
	elem->j = -1;
	str = malloc(sizeof(char)
			* (ft_strlen(tmp) + ft_strlen(elem->fullcmd) + 1));
	if (!str || add_address(&elem->p_data->collector, str))
		return (NULL);
	while (tmp[++i])
		str[i] = tmp[i];
	str[i] = 0;
	if (tmp)
		free(tmp);
	elem->redir.i = ft_strlen(str) - 1;
	if (elem->i == (int)ft_strlen(elem->fullcmd))
		elem->i++;
	return (str);
}

void	place_var(t_parse *elem, int isarg, int nb)
{
	if (isarg == 0)
		elem->cmd = \
		replace_in_cmdarg(elem, elem->var_val, elem->cmd);
	if (isarg == 1)
		elem->args[nb] = \
		replace_in_cmdarg(elem, elem->var_val, elem->args[nb]);
	if (isarg == 2)
		elem->redir.in = \
		replace_in_redir(elem, elem->var_val, elem->redir.in);
	if (isarg == 3)
		elem->redir.out1 = \
		replace_in_redir(elem, elem->var_val, elem->redir.out1);
	if (isarg == 4)
		elem->redir.out2 = \
		replace_in_redir(elem, elem->var_val, elem->redir.out2);
}
