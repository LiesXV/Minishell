/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:27:05 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/07/03 16:22:08 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void free_tab(void **tab)
{
	int	i;
	
	i = 0;
	while (tab[i] !=NULL)
	{
		free(tab[i]);
		i++;
	}
}

int	is_quotes_openned(char *str, char c)
{
	int	i;
	int	count;
	
	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == c)
			count++;
		i++;
	}
	if (count % 2 == 0)
		return (0);
	else
		return (1);
}

void	printtab(char **tab)
{
	int	i = 0;

	while (tab[i] != NULL)
	{
		ft_printf("\t%d : %s\n", i, tab[i]);
		i++;
	}
	return ;
}

int	input_handling(char *input, t_data *data)
{
	char	**fullcommands;

	if (is_quotes_openned(input, 34) || is_quotes_openned(input, 39))
		return (ft_putstr_fd("\x1B[31merror\x1B[0m : unclosed quotes.\n", 1), FAILURE);
	fullcommands = ft_multisplit(input, ";"); //split toutes les commandes
	if (!fullcommands)
		return (FAILURE);
	data->cmd_lst = formating(fullcommands);
	while ((*data->cmd_lst))
	{
		handle_exec(data);
		(*data->cmd_lst) = (*data->cmd_lst)->next;
	}
	return (SUCCESS);
}
