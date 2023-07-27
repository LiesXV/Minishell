/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 15:07:08 by lmorel            #+#    #+#             */
/*   Updated: 2023/07/27 02:01:36 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_signals(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (sig == SIGQUIT)
	{
		ft_putstr_fd("\b\b  \b\b", 1);
	}
}

int	main(int ac, char **av, char **envp)
{
	char		*input;
	t_data		data;

	(void)av;
	if (ac > 1)
		return (printf("\033[1m\033[31mNo args required.\033[0m"), FAILURE);
	data.envp = envp;
	data.env = get_env(&data);
	data.path = getenv("PATH");
	if (!data.path)
		return (FAILURE);
	data.collector = NULL;
	if (signal(SIGINT, handle_signals) == SIG_ERR)
		printf("failed to find signal\n");
	if (signal(SIGQUIT, handle_signals) == SIG_ERR)
		printf("failed to find signal\n");
	input = NULL;
	while (1)
	{
		input = readline(PROMPT);
		if (!input)
			break ;
		if (!only_spaces(input))
			add_history(input);
		add_address(&data.collector, input);
		if (!do_nothing(input) && invalid_input(input, 0, '|') != -1 && invalid_input(input, 0, ';') != -1 && invalid_input(input, 0, '&') != -1 && invalid_input(input, 0, ')') != -1)
			input_handling(input, &data);
	}
	free_all(&data.collector);
	return (SUCCESS);
}

/*
	RESTE A FAIRE

	split qui formate les pipes
	signaux
	gerer les free avec le gb collector
	
	BUG listing 
		?> env hola
		/> pwd -p
*/
