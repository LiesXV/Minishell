/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_checking.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 22:39:51 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/08 18:21:26 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	do_nothing(char *input)
{
	int	i;

	i = 0;
	while (contains(input[i], " \\:!\t\n\r\v\f"))
	{
		if (i > 0 && contains(input[i], ":!") && contains(input[i - 1], ":!"))
			break ;
		i++;
	}
	if (i == ((int)ft_strlen(input)))
		return (1);
	else 
		return (0);
}

int	invalid_input_arrow(char *input, char c)
{
	int	i;
	int	j;

	i = 0;
	while (input[i])
	{
		j = 0;
		i = error_in_quotes(input, i);
		if (i == (int)ft_strlen(input))
			break ;
		while (input[i] && (input[i] == c || input[i] == ' '))
		{
			if (input[i] == c)
				j++;
			i++;
			if (j == 3)
				return (put_error_msg(c, i + 1, input));
			if (j > 3)
				return (put_error_msg(c, i, input));
		}
		if (i == (int)ft_strlen(input))
			break ;
		i++;
	}
	return (0);
}

int	invalid_input_nl(char *input)
{
	int	i;

	i = 0;
	while (input[i])
		i++;
	i--;
	if (input[i] == '>' || input[i] == '<')
		return (put_error_msg('n', i, input));
	i++;
	while (input[i--] && (input[i] == ' ' 
			|| input[i] == '>' || input[i] == '<'))
	{
		i = error_in_quotes(input, i);
		if (i == (int)ft_strlen(input))
			break ;
		if (input[i] == '>' || input[i] == '<')
			return (put_error_msg('n', i, input));
	}
	return (0);
}

int	force_invalid_input(char *input, int i, char c)
{
	i--;
	if (input[i] && input[i] == '|')
		return (put_error_msg(c, i, input));
	if (invalid_input_arrow(input, '>') == -1 
		|| invalid_input_arrow(input, '<') == -1 
		|| invalid_input_nl(input) == -1)
	{
		g_end_status = 2;
		return (-1);
	}
	return (0);
}

int	invalid_input(char *input, int i, char c)
{
	if (input[0] == c)
		return (put_error_msg(c, 0, input));
	while (input[++i] && (input[i] == ' ' 
			|| input[i] == '>' || input[i] == '<' || input[i] == c))
		if (input[i] == c)
			return (put_error_msg(c, i, input));
	while (input[i])
	{
		i = error_in_quotes(input, i);
		if (i == (int)ft_strlen(input))
			break ;
		if (input[i] && input[i] == c)
		{
			while (input[++i] && (input[i] == ' ' 
					|| input[i] == '>' || input[i] == '<' || input[i] == c))
				if (input[i] == c)
					return (put_error_msg(c, i, input));
			if (input[i] == 0)
				break ;
		}
		i++;
	}
	return (force_invalid_input(input, i, c));
}
