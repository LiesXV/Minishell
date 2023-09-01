/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 15:07:08 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/01 15:12:09 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_end_status;

void	handle_signals_after(int sig)
{
	(void)sig;
}

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

// faire exit de minishell en cas de fail signaux ou pas ?
int init_signals(int token)
{
	if (token == 0)
	{
		if (signal(SIGINT, handle_signals) == SIG_ERR)
			return (printf("failed to find signal\n"), FAILURE);
		if (signal(SIGQUIT, handle_signals) == SIG_ERR)
			return (printf("failed to find signal\n"), FAILURE);
	}
	else
	{
		if (signal(SIGINT, handle_signals_after) == SIG_ERR)
			return (printf("failed to find signal\n"), FAILURE);
		if (signal(SIGQUIT, handle_signals_after) == SIG_ERR)
			return (printf("failed to find signal\n"), FAILURE);
		if (DEBUG) printf("SIGNALS CHANGED!\n");
	}
	return (SUCCESS);
}

void	closefds_pipe(t_data *data)
{
	t_piplist	*piplist;

	piplist = *(*data->cmd_lst)->piplist;

	while (piplist != NULL)
	{
		if (piplist->redir.sstdin > 2)
			close(piplist->redir.sstdin);
		if (piplist->redir.sstdout > 2)
			close(piplist->redir.sstdout);
		if (piplist->redir.sstderr > 2)
			close(piplist->redir.sstderr);
		piplist = piplist->next;
	}
}

int	main(int ac, char **av, char **envp)
{
	char		*input;
	// char		*prompt;
	t_data		data;

	(void)av;
	if (ac > 1)
		return (printf("\033[1m\033[31mNo args required.\033[0m"), FAILURE);
	g_end_status = 0;
	data.collector = NULL;
	data.envp = envp;
	data.env = get_env(&data);
	// add_address(&data.collector, data.env);
	data.path = getenv("PATH");
	printf("\033[1m\033[31mENTERING MINISHELL\033[0m\n");
	input = NULL;
	while (1)
	{
		init_signals(0);
		input = readline(PROMPT);
		if (!input || add_address(&data.collector, input) == FAILURE)
			return (closefds_pipe(&data), free_all(&data.collector), free_all_env(&data.env), FAILURE);
		if (!only_spaces(input))
			add_history(input);
		if (!do_nothing(input) && invalid_input(input, 0, '|') != -1 && invalid_input(input, 0, ';') != -1 && invalid_input(input, 0, '&') != -1 && invalid_input(input, 0, ')') != -1)
			input_handling(input, &data);
	}
	closefds_pipe(&data);
	free_all_env(&data.env);
	free_all(&data.collector);
	return (SUCCESS);
}




/*
	RESTE A FAIRE

	gerer les free avec le gb collector
	remove les fonction interdites
	
	BUG listing 
		?> env hola
		/> pwd -p
*/
