/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:27:05 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/06/20 19:22:28 by lmorel           ###   ########.fr       */
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
	t_parse	**list;
	int		i;

	if (is_quotes_openned(input, 34) || is_quotes_openned(input, 39))
		return (ft_putstr_fd("\x1B[31merror\x1B[0m : unclosed quotes.\n", 1), FAILURE);
	i = 0;
	fullcommands = ft_multisplit(input, ";"); //split toutes les commandes
	if (!fullcommands)
		return (FAILURE);
	list = formating(fullcommands);
	(void)list;
	while (fullcommands[i] != NULL)
	{
		add_address(&data->collector, fullcommands[i]);
		if (is_builtin(fullcommands[i], data) == 1)
		{
			data->cmd_lst = add_cmd(fullcommands[i], data);
			//if (!data->cmd_lst)
			//	printf("marche pas connard\n");
			get_cmd(data);
		}
		i++;
		wait(NULL);
	}
	add_address(&data->collector, fullcommands);
	return (SUCCESS);
}
