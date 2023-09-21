/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix_spaces.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 19:24:30 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/21 18:37:05 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	form_new(char **new, char *str, size_t i)
{
	size_t	j;
	size_t	k;

	k = 0;
	j = 0;
	while (str[i])
	{
		if ((contains(str[i], "\t\n\r\v\f ") && !k))
		{
			k = 1;
			(*new)[j++] = str[i];
		}
		else if (!contains(str[i], "\t\n\r\v\f "))
		{
			k = 0;
			(*new)[j++] = str[i];
		}
		i++;
	}
	(*new)[j] = '\0';
}

int	add(char *fullcmd, char *copy, char **new, int count)
{
	int	j;

	j = 0;
	if (count == -1)
	{
		if (copy)
			free(copy);
		return (-1);
	}
	if (add_space_before_condition(fullcmd, count) == 1)
	{
		while (copy[j])
		{
			(*new)[0] = ' ';
			(*new)[j + 1] = copy[j];
			j++;
		}
		(*new)[j + 1] = '\0';
	}
	return (1);
}

static char	*add_spaces(char *tmp, char *fullcmd, int count, char *new)
{
	if (add_space_before(tmp, fullcmd, count, &new) == -1)
	{
		if (tmp)
			free(tmp);
		return (new);
	}
	add_space_after(tmp, fullcmd, count, &new);
	if (tmp)
		free(tmp);
	return (new);
}

static size_t	len_no_spaces(char *str)
{
	size_t		i;
	size_t		len;
	char		found_space;

	len = ft_strlen(str);
	while (str && len > 0 && (len - 1) > 0 && str[len - 1] 
		&& contains(str[len - 1], "\t\n\r\v\f "))
		len--;
	str[len] = '\0';
	i = 0;
	while (str[i] && contains(str[i], "\t\n\r\v\f "))
		i++;
	len -= i;
	i--;
	found_space = 0;
	while (str[++i])
	{
		if (contains(str[i], "\t\n\r\v\f ") && !found_space)
			found_space = 1;
		else if (contains(str[i], "\t\n\r\v\f "))
			len--;
		else
			found_space = 0;
	}
	return (len);
}

char	*handle_var_spaces(char *str, char *fullcmd, int count)
{
	char	*new;
	char	*tmp;
	size_t	i;
	size_t	len;

	tmp = ft_strdup(str);
	if (!tmp)
		return (NULL);
	len = len_no_spaces(str);
	new = malloc(sizeof(char) * (len + 3));
	if (!new)
		return (free(tmp), NULL);
	i = ft_strlen(str);
	if (str && i > 0)
		while ((i - 1) > 0 && str[i - 1] && contains(str[i - 1], "\t\n\r\v\f "))
			i--;
	str[i] = 0;
	i = 0;
	while (str[i] && contains(str[i], "\t\n\r\v\f "))
		i++;
	form_new(&new, str, i);
	return (add_spaces(tmp, fullcmd, count, new));
}
