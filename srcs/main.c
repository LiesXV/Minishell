/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 15:07:08 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/27 12:48:27 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_end_status;

void	free_and_exit(t_data *data)
{
	free_all_env(&data->env);
	free_all(&data->collector);
	exit(g_end_status);
}

void	closefds_pipe(t_data *data)
{
	t_piplist	*piplist;
	t_parse		*cur;

	cur = (*data->cmd_lst);
	if (!cur->piplist)
		return ;
	piplist = *(*cur).piplist;
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

int	init_data(t_data *data, char **envp)
{
	data->collector = NULL;
	data->envp = envp;
	data->env = get_env(data);
	if (!data->env)
		return (FAILURE);
	data->path = getenv("PATH");
	return (SUCCESS);
}

int	main(int ac, char **av, char **envp)
{
	char		*input;
	t_data		data;

	(void)av;
	if (ac > 1)
		return (printf("\033[1m\033[31mNo args required.\033[0m"), FAILURE);
	if (init_data(&data, envp) == FAILURE)
		return (ft_putstr_fd("error with data initialisation\n", 2), FAILURE);
	input = NULL;
	while (1)
	{
		init_signals(0);
		input = readline(PROMPT);
		if (!input || add_address(&data.collector, input) == FAILURE)
			break ;
		if (input)
			add_history(input);
		if (!do_nothing(input) && invalid_input(input, 0, '|') != -1 && \
		invalid_input(input, 0, ';') != -1 && invalid_input(input, 0, '&') \
		!= -1 && invalid_input(input, 0, ')') != -1)
			input_handling(input, &data);
	}
	return (free_all_env(&data.env), free_all(&data.collector), g_end_status);
}
