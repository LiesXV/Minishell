/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 17:36:37 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/27 12:45:52 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	error_exit(char *name, char *err)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(name, 2);
	if (errno != 0)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
	}
	else
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(err, 2);
		ft_putstr_fd("\n", 2);
	}
	g_end_status = 1;
	return (-1);
}

int	quotes_error(char c)
{
	ft_putstr_fd("minishell: unexpected error while looking for matching '", 2);
	ft_putchar_fd(c, 2);
	ft_putstr_fd("'\n", 2);
	g_end_status = 1;
	return (-1);
}

int	pipe_error(char c)
{
	ft_putstr_fd("minishell: unexpected error around '", 2);
	ft_putchar_fd(c, 2);
	ft_putstr_fd("'\n", 2);
	g_end_status = 1;
	return (-1);
}

int	put_error_msg(char c, int i, char *input)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("syntax error near unexpected token '", 2);
	if (c == 'n')
		ft_putstr_fd("newline\'\n", 2);
	else if (c != ')' && (input[i + 1] == c || (i > 0 && input[i - 1] == c)))
	{
		ft_putchar_fd(c, 2);
		ft_putchar_fd(c, 2);
		ft_putstr_fd("\'\n", 2);
	}
	else
	{
		ft_putchar_fd(c, 2);
		ft_putstr_fd("\'\n", 2);
	}
	g_end_status = 2;
	return (-1);
}

int	error_in_quotes(char *input, int i)
{
	char	quote;

	while (input[i] && (input[i] == '\'' || input[i] == '\"'))
	{
		quote = input[i];
		i++;
		while (input[i] && input[i] != quote)
		{
			if (input[i] && input[i] == '\\')
				i++;
			i++;
		}
		if (i == ((int)ft_strlen(input)))
			break ;
		else
			i++;
	}
	return (i);
}
