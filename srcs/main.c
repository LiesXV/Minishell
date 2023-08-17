/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 15:07:08 by lmorel            #+#    #+#             */
/*   Updated: 2023/08/17 23:11:00 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_end_status;

void	handle_signals(int sig)
{
	g_end_status += sig;
	if (sig == SIGINT)
	{
		g_end_status = 130;
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
		ft_putstr_fd("\b\b  \b\b", 1);
}

// void	create_env(&data)
// {
// 	printf("\tttyslot %d", ttyslot());
// }

int	main(int ac, char **av, char **envp)
{
	char		*input;
	char		*prompt;
	t_data		data;

	(void)av;
	if (ac > 1)
		return (printf("\033[1m\033[31mNo args required.\033[0m"), FAILURE);
	g_end_status = 0;
	data.collector = NULL;
	data.envp = envp;
	data.env = get_env(&data);
	add_address(&data.collector, data.env);
	printf("\033[1m\033[31mENTERING MINISHELL\033[0m\n");
	data.path = getenv("PATH");
	if (signal(SIGINT, handle_signals) == SIG_ERR)
		printf("failed to find signal\n");
	if (signal(SIGQUIT, handle_signals) == SIG_ERR)
		printf("failed to find signal\n");
	input = NULL;
	while (1)
	{
		dup2(1,1);
		dup2(0,0);
		prompt = ft_strdup(PROMPT);
		input = readline(prompt);
		if (!input)
			return (free_all(&data.collector), FAILURE);
		add_address(&data.collector, input);
		if (!only_spaces(input))
			add_history(input);
		if (!do_nothing(input) && invalid_input(input, 0, '|') != -1 && invalid_input(input, 0, ';') != -1 && invalid_input(input, 0, '&') != -1 && invalid_input(input, 0, ')') != -1)
		{
			input_handling(input, &data);
		}
		free(prompt);
	}
	free_all_env(&data);
	free_all(&data.collector);
	return (SUCCESS);
}

/*
	RESTE A FAIRE

	signaux
	gerer les free avec le gb collector
	can't echo an exported var
	
	BUG listing 
		?> env hola
		/> pwd -p
*/
