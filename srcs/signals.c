/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 15:07:08 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/27 17:10:52 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_signals_after(int sig)
{
	(void)sig;
}

void	handle_signals(int sig)
{
	if (sig == SIGINT)
	{
		g_end_status = 130;
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
	{
		g_end_status = 131;
		ft_putstr_fd("\b\b  \b\b", 1);
	}
}

void	hd_sig(int sig)
{
	if (sig == SIGINT)
	{
		g_end_status = 130;
		printf("\n");
		close(0);
	}
}

int	init_signals(int token)
{
	if (token == 0)
	{
		if (signal(SIGINT, handle_signals) == SIG_ERR)
			return (printf("failed to find signal\n"), FAILURE);
		if (signal(SIGQUIT, handle_signals) == SIG_ERR)
			return (printf("failed to find signal\n"), FAILURE);
	}
	else if (token == 1)
	{
		if (signal(SIGINT, handle_signals_after) == SIG_ERR)
			return (printf("failed to find signal\n"), FAILURE);
		if (signal(SIGQUIT, handle_signals_after) == SIG_ERR)
			return (printf("failed to find signal\n"), FAILURE);
	}
	else if (token == 15)
	{
		if (signal(SIGINT, hd_sig) == SIG_ERR)
			return (printf("failed to find signal\n"), FAILURE);
		if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
			return (printf("failed to find signal\n"), FAILURE);
	}
	return (SUCCESS);
}
