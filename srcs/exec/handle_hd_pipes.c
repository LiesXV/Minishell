/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_hd_pipes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 19:07:54 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/27 12:00:05 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	open_a_tmp_pipes(t_piplist *cur)
{
	char	*file;
	int		fd;
	int		i;
	char	*itoa;

	i = 0;
	file = ft_strdup(".heredoc_tmp");
	if (!file)
		return (-1);
	while (file && access(file, F_OK) == 0 && ++i < 256)
	{
		free(file);
		itoa = ft_itoa(i);
		if (!itoa)
			return (FAILURE);
		file = ft_strjoin(".heredoc_tmp", itoa);
		free(itoa);
		if (!file)
			return (FAILURE);
	}
	fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	cur->redir.in = ft_strdup(file);
	if (fd == -1 || i > 255 || !cur->redir.in)
		return (free(file), -1);
	return (free(file), fd);
}

int	read_end(int stdin_copy, char *line)
{
	dup2(stdin_copy, 0);
	close(stdin_copy);
	init_signals(0);
	get_next_line(-1);
	free(line);
	if (g_end_status == 130)
		return (g_end_status = 0, 1);
	return (g_end_status = 0, 0);
}

int	read_hd_pipes(char *hd, int fd)
{
	char	*line;
	int		stdin_copy;

	line = NULL;
	stdin_copy = dup(0);
	g_end_status = 99;
	signal(SIGINT, SIG_IGN);
	init_signals(15);
	while (1)
	{
		line = readline(">");
		if (!line)
		{
			if (g_end_status == 130)
				break ;
			else if (printf("minishell : warning: here-document \
				delimited by end-of-file\n"))
				break ;
		}
		if (!ft_strncmp(hd, line, ft_strlen(hd) + 1))
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	return (read_end(stdin_copy, line));
}

int	read_input_pipes(t_data *data, t_piplist *cur)
{
	int		file;
	int		i;

	i = -1;
	file = open_a_tmp_pipes(cur);
	if (file < 0)
		return (-1);
	if (add_address(&data->collector, cur->redir.in) == 1)
		return (-1);
	while (cur->redir.hd[++i])
	{
		if (read_hd_pipes(cur->redir.hd[i], file))
			break ;
		if (cur->redir.hd[i + 1])
		{
			close(file);
			unlink(cur->redir.in);
			file = open(cur->redir.in, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		}
		else
			break ;
	}
	close(file);
	cur->redir.sstdin = open(cur->redir.in, O_RDONLY);
	return (0);
}

void	handle_hd_pipes(t_data *data, t_piplist *cur)
{
	while (cur)
	{
		if (cur->redir.hd)
		{
			if (read_input_pipes(data, cur) == -1)
				printf("error creating here doc\n");
		}
		cur = cur->next;
	}
	return ;
}
