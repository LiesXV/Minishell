/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 15:07:08 by lmorel            #+#    #+#             */
/*   Updated: 2023/06/14 19:18:35 by ibenhaim         ###   ########.fr       */
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
}

int	main(int ac, char **av, char **envp)
{
	char		*input;
	t_data		data;

	(void)av;
	if (ac > 1)
		return (printf("\033[1m\033[31mNo args required.\033[0m"), FAILURE);
	data.envp = envp;
	data.path = getenv("PATH");
	if (!data.path)
		return (FAILURE);
	data.collector = NULL;
	if (signal(SIGINT, handle_signals) == SIG_ERR)
		printf("failed to find signal\n");
	while (1)
	{
		input = readline(PROMPT);
		if (!input)
			break ;
		add_history(input);
		add_address(&data.collector, input);
		input_handling(input, &data);
	}
	free_all(&data.collector);
	return (SUCCESS);
}
