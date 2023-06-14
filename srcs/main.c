/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 15:07:08 by lmorel            #+#    #+#             */
/*   Updated: 2023/06/14 17:50:42 by ibenhaim         ###   ########.fr       */
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
		return (1);
	data.envp = envp;
	data.path = getenv("PATH");
	printf("path [%s]\n", data.path);
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
		//input_handling(input, data);
	}
	free_all(&data.collector);
	return (0);
}
