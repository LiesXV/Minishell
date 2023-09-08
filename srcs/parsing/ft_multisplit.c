/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_multisplit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 18:10:51 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/08 18:28:42 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	contains(char c, char *s)
{
	while (*s)
	{
		if (c == *s)
			return (1);
		s++;
	}
	return (0);
}

static int	nbwords(char *str, char *s)
{
	int	count;
	int	i;
	int	prev_i;

	count = 0;
	i = 0;
	while (str[i])
	{
		while (contains(str[i], s) && str[i])
			i++;
		prev_i = i;
		while (!contains(str[i], s) && str[i])
			i++;
		if (i > prev_i)
			count++;
	}
	return (count);
}

static void	free_s_tab(char **strs)
{
	int	i;

	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

char	**fill_tab(char **strs, char *str, char *s)
{
	int	i;
	int	prev_i;
	int	k;

	i = 0;
	k = 0;
	while (str[i])
	{
		while (contains(str[i], s) && str[i])
			i++;
		prev_i = i;
		while (!contains(str[i], s) && str[i])
			i++;
		if (i > prev_i)
		{
			strs[k] = ft_substr(str, prev_i, i - prev_i);
			if (!strs[k])
				return (free_s_tab(strs), NULL);
			k++;
		}
	}
	strs[k] = NULL;
	return (strs);
}

char	**ft_multisplit(char *str, char *s)
{
	char	**strs;

	if (!str || !s)
		return (NULL);
	strs = malloc(sizeof(char *) * (nbwords(str, s) + 1));
	if (!strs)
		return (NULL);
	strs = fill_tab(strs, str, s);
	if (!strs)
		return (NULL);
	return (strs);
}
