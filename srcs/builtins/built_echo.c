/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 19:24:21 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/24 17:54:23 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	built_echo_end(char	**args, int	*i, t_redir redir)
{
	while (args[*i])
	{
		if (ft_putstr_fd(args[*i], redir.sstdout) \
			!= (int)ft_strlen(args[*i]))
			return (ft_putstr_fd("write error", 2), FAILURE);
		if (args[*i + 1])
			ft_putchar_fd(' ', redir.sstdout);
		*i = *i + 1;
	}
	return (SUCCESS);
}

int	built_echo(char **args, t_redir redir)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (args[i] && !ft_strncmp(args[i], "echo", 4) \
		&& ft_strlen(args[i]) == 4)
		i++;
	if (!args[i])
		return (g_end_status = 0, ft_putchar_fd('\n', redir.sstdout), SUCCESS);
	if (args[i] && !ft_strncmp(args[i], "-n", 2))
	{
		j = i;
		i++;
		while (args[i] && !ft_strncmp(args[i], "-n", 2))
			i++;
	}
	if (built_echo_end(args, &i, redir) == FAILURE)
		return (g_end_status = 0, SUCCESS);
	if (!(!ft_strncmp(args[j], "-n", 2)))
		ft_putchar_fd('\n', redir.sstdout);
	return (g_end_status = 0, SUCCESS);
}
