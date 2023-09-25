/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_hd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 18:43:02 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/09/25 15:28:01 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	open_a_tmp(t_parse *cur)
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
	if (i > 255 || !cur->redir.in || fd == -1)
		return (free(file), -1);
	return (free(file), fd);
}

void	read_hd(char *hd, int fd)
{
	char	*line;

	line = NULL;
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(0);
		if (!line)
			exit(g_end_status);
		line[ft_strlen(line) - 1] = 0;
		if (!ft_strncmp(hd, line, ft_strlen(hd) + 1))
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	get_next_line(-1);
	free(line);
}

int	read_input(t_data *data, t_parse *cur)
{
	int		file;
	int		i;

	i = 0;
	file = open_a_tmp(cur);
	if (file < 0)
		return (-1);
	if (add_address(&data->collector, cur->redir.in) == 1)
		return (-1);
	while (cur->redir.hd[i])
	{
		read_hd(cur->redir.hd[i], file);
		if (cur->redir.hd[i + 1])
		{
			close(file);
			file = open(cur->redir.in, O_WRONLY, O_TRUNC);
		}
		else
			break ;
		i++;
	}
	close(file);
	data->infile = open(cur->redir.in, O_RDONLY);
	return (0);
}

void	handle_hd(t_data *data, t_parse *cur)
{
	if (read_input(data, cur) == -1)
		printf("error creating here doc\n");
	return ;
}
