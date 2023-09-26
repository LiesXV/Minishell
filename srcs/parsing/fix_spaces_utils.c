/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix_spaces_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 19:28:12 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/08 19:36:25 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	only_spaces(char *str)
{
	int	i;

	i = 0;
	while (contains(str[i], " \t\n\r\v\f"))
		i++;
	if (i == (int)ft_strlen(str))
		return (1);
	return (0);
}

int	mid_space(char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ')
		i++;
	while (str[i] && str[i] != ' ')
		i++;
	while (str[i] == ' ')
	{
		i++;
		if (str[i] && str[i] != ' ')
			return (1);
	}
	return (0);
}

int	after_var(int count, char *fullcmd)
{
	count--;
	while (fullcmd[count] && fullcmd[count] != ' ')
	{
		if (fullcmd[count] == '$' && fullcmd[count - 1] != '\\')
			return (1);
		count--;
	}
	return (0);
}

int	add_space_before_condition(char *fullcmd, int count)
{
	if (after_var(count, fullcmd) || fullcmd[count - 1] == '"'
		|| fullcmd[count - 1] == '\'' || fullcmd[count - 1] == '/'
		|| fullcmd[count - 1] == '@' || fullcmd[count - 1] == ','
		|| fullcmd[count - 1] == '[' || fullcmd[count - 1] == ']'
		|| (fullcmd[count - 1] == ' ' && fullcmd[count - 2] == '\\'))
		return (1);
	return (0);
}

int	add_space_before_util(char *fullcmd, int count, char *tmp)
{
	if (fullcmd[count + 1] == ' ' && only_spaces(tmp))
		return (-1);
	while (fullcmd[count] && fullcmd[count] != '$'
		&& fullcmd[count - 1] != '\\')
		count--;
	if (fullcmd[count - 1] == ' ' && only_spaces(tmp))
		return (-1);
	return (count);
}
