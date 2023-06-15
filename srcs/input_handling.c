/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:27:05 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/06/15 19:06:33 by ibenhaim         ###   ########.fr       */
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
	int openned;
	
	openned = false;
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			openned = true;
		//while (str[i] != c && str[i])
		//	i++;
		if (openned == true && str[i] == c)
			openned = false;
		i++;
	}
	printf("%c - %d\n", c, openned);
	return (openned);
}

int	input_handling(char *input, t_data *data)
{
	char **fullcommands;
	int		i;
	
	if (is_quotes_openned(input, 34) || is_quotes_openned(input, 39))
		return (ft_putstr_fd("\x1B[31merror\x1B[0m : unclosed quotes.\n", 1), FAILURE);
	i = 0;
	fullcommands = ft_split(input, '|'); //split toutes les commandes
	if (!fullcommands)
		return (FAILURE);
	while (fullcommands[i] != NULL)
	{
		add_address(&data->collector, fullcommands[i]);
		if(is_builtin(fullcommands[i], data) == 1)
		{
			data->cmd_lst = add_cmd(fullcommands[i], data);
			if (!data->cmd_lst)
				printf("marche pas connard\n");
			get_cmd(data);
		}
		i++;
		wait(NULL);
	}
	add_address(&data->collector, fullcommands);
	return (SUCCESS);
}
