/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 16:28:39 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/04 13:33:26 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	only_spaces(char *str)
{
	int i;

	i = 0;
	while (contains(str[i], " \t\n\r\v\f"))
		i++;
	if (i == (int)ft_strlen(str))
		return (1);
	return (0);
}

int	dollar_error(t_parse *elem, int isarg, int nb)
{
	int err;
	
	err = 0;
	while (elem->fullcmd[elem->i] && elem->fullcmd[elem->i] == '$' && (elem->fullcmd[elem->i + 1] == 0 || elem->fullcmd[elem->i + 1] == '$' || (elem->i > 0 && elem->fullcmd[elem->i - 1] == '$')))
	{
		if (isarg == 0)
			elem->cmd[++elem->j] = elem->fullcmd[elem->i];
		if (isarg == 1)
			elem->args[nb][++elem->j] = elem->fullcmd[elem->i];
		elem->i++;
		if (ft_strlen(elem->fullcmd) == 1 && elem->fullcmd[0] == '$')
			return (1);
		err++;
	}
	if (err > 0)
	{
		elem->i--;
		return (1);
	}
	return (0);
}

int init_var_str(t_parse *elem)
{
	elem->var = NULL;
	elem->var_val = NULL;
	elem->var = malloc(sizeof(char) * (ft_strlen(elem->fullcmd) + 1));
	if (!elem->var || add_address(&elem->p_data->collector, elem->var) == 1)
		return (FAILURE);
	return (SUCCESS);
}

int find_var(t_parse *elem, int state)
{
	int count;

	count = -1;
	if (state == 0)
		elem->i++;
	if (state == 0 && (elem->fullcmd[elem->i] == '\'' || elem->fullcmd[elem->i] == '"'))
	{
		elem->i--;
		return (FAILURE);
	}
	while (elem->fullcmd[elem->i] && !contains(elem->fullcmd[elem->i], "\\ $[]\",'/=|@"))
	{
		if (state == 0 && (elem->fullcmd[elem->i] == '\'' || elem->fullcmd[elem->i] == '"'))
			return (FAILURE);
		elem->var[++count] = elem->fullcmd[elem->i];
		elem->i++;
	}
	elem->var[count + 1] = 0;
	return (SUCCESS);
}

int test_value(t_parse *elem)
{
	if (!elem->var_val)
	{
		if (contains(elem->fullcmd[elem->i], "\\ $[]\",'/=|@"))
			elem->i--;
		if (elem->fullcmd[elem->i] == ' ' && elem->fullcmd[elem->i - 1] != '\\')
			return (-2);
		if (elem->fullcmd[elem->i] == '$' && elem->fullcmd[elem->i - 1] != '\\')
			elem->i--;
		return (1);
	}
	return (0);
}

char *replace_in_cmdarg(t_parse *elem, char *var, char *str)
{
	char	*tmp;
	int 	i;
	
	i = -1;
	tmp = NULL;
	tmp = ft_strjoin(str, var);
	if (!tmp)
		return (NULL);
	if (str)
		free (str);
	elem->j = -1;
	str = malloc(sizeof(char) * (ft_strlen(tmp) + ft_strlen(elem->fullcmd) + 1));
	if (!str)
		return (NULL);
	while (tmp[++i])
		str[++elem->j] = tmp[i];
	if (tmp)
		free(tmp);
	str[++elem->j] = 0;
	return (str);
}

char *replace_in_redir(t_parse *elem, char *var, char *str)
{
	char	*tmp;
	int 	i;
	
	i = -1;
	tmp = NULL;
	str[elem->redir.i + 1] = 0;
	tmp = ft_strjoin(str, var);
	if (!tmp)
		return (NULL);
	if (str)
		free (str);
	elem->j = -1;
	str = malloc(sizeof(char) * (ft_strlen(tmp) + ft_strlen(elem->fullcmd) + 1));
	if (!str)
		return (NULL);
	while (tmp[++i])
		str[i] = tmp[i];
	str[i] = 0;
	if (tmp)
		free(tmp);
	elem->redir.i = ft_strlen(str) - 1;
	if (elem->i == (int)ft_strlen(elem->fullcmd))
		elem->i++;
	return (str);
}

void place_var(t_parse *elem, int isarg, int nb)
{
	if (isarg == 0)
		elem->cmd = replace_in_cmdarg(elem, elem->var_val, elem->cmd);
	if (isarg == 1)
		elem->args[nb] = replace_in_cmdarg(elem, elem->var_val, elem->args[nb]);
	if (isarg == 2)
		elem->redir.in = replace_in_redir(elem, elem->var_val, elem->redir.in);
	if (isarg == 3)
		elem->redir.out1 = replace_in_redir(elem, elem->var_val, elem->redir.out1);
	if (isarg == 4)
		elem->redir.out2 = replace_in_redir(elem, elem->var_val, elem->redir.out2);
}

void	form_new(char **new, char *str, size_t i)
{
	size_t j;
	size_t k;

	k = 0;
	j = 0;
	while (str[i])
	{
		if ((contains(str[i], "\t\n\r\v\f ") && !k))
		{
			k = 1;
			(*new)[j++] = str[i];
		}
		else if (!contains(str[i], "\t\n\r\v\f "))
		{
			k = 0;
			(*new)[j++] = str[i];
		}
		i++;
	}
	(*new)[j] = '\0';
}

int				after_var(int count, char *fullcmd)
{
	count--;
	while (fullcmd[count] && fullcmd[count] != ' ')
	{
		if (fullcmd[count] == '$' && fullcmd[count - 1] != '\\')
			return (1);
		count--;
	}
	return (0);
}

int				add_space_before_condition(char *fullcmd, int count)
{
	if (after_var(count, fullcmd) || fullcmd[count - 1] == '"' || fullcmd
		[count - 1] == '\'' || fullcmd[count - 1] == '/' || fullcmd[count - 1] ==
		'@' || fullcmd[count - 1] == ',' || fullcmd[count - 1] == '[' ||
		fullcmd[count - 1] == ']' || (fullcmd[count - 1] == ' ' &&
		fullcmd[count - 2] == '\\'))
		return (1);
	return (0);
}

int				add_space_before_util(char *fullcmd, int count, char *tmp)
{
	if (fullcmd[count + 1] == ' ' && only_spaces(tmp))
		return (-1);
	while (fullcmd[count] && fullcmd[count] != '$' && fullcmd[count - 1] != '\\')
		count--;
	if (fullcmd[count - 1] == ' ' && only_spaces(tmp))
		return (-1);
	return (count);
}

static int		add(char *fullcmd, char *copy, char **new, int count)
{
	int j;

	j = 0;
	if (count == -1)
	{
		if (copy)
			free(copy);
		return (-1);
	}
	if (add_space_before_condition(fullcmd, count) == 1)
	{
		while (copy[j])
		{
			(*new)[0] = ' ';
			(*new)[j + 1] = copy[j];
			j++;
		}
		(*new)[j + 1] = '\0';
	}
	return (1);
}

int				add_space_before(char *tmp, char *fullcmd, int count, char **new)
{
	int		i;
	char	*copy;

	i = 0;
	copy = NULL;
	copy = ft_strdup(*new);
	count--;
	if (tmp[i] == ' ')
	{
		count = add_space_before_util(fullcmd, count, tmp);
		if (add(fullcmd, copy, new, count) == -1)
			return (-1);
	}
	if (copy)
		free(copy);
	return (1);
}

void			add_space_after(char *tmp, char *fullcmd, int count, char **new)
{
	int i;
	int j;

	j = ft_strlen(*new);
	if (!tmp || ((i = (ft_strlen(tmp) - 1)) < 0))
		return ;
	if (tmp[i] == ' ' && after_var(count, fullcmd) && fullcmd[count] == '$')
		return ;
	if (tmp[i] == ' ' && contains(fullcmd[count], "\"'/@,$[]\\"))
	{
		(*new)[j] = ' ';
		(*new)[j + 1] = '\0';
	}
}

static char		*add_spaces(char *tmp, char *fullcmd, int count, char *new)
{
	if (add_space_before(tmp, fullcmd, count, &new) == -1)
	{
		if (tmp)
			free(tmp);
		return (new);
	}
	add_space_after(tmp, fullcmd, count, &new);
	if (tmp)
		free(tmp);
	return (new);
}

static size_t	len_no_spaces(char *str)
{
	size_t		i;
	size_t		len;
	char		found_space;

	len = ft_strlen(str);
	if (str && (len) > 0)
		while ((len - 1) > 0 && str[len - 1] && contains(str[len - 1], "\t\n\r\v\f "))
			len--;
	str[len] = '\0';
	i = 0;
	while (str[i] && contains(str[i], "\t\n\r\v\f "))
		i++;
	len -= i;
	i--;
	found_space = 0;
	while (str[++i])
	{
		if (contains(str[i], "\t\n\r\v\f ") && !found_space)
			found_space = 1;
		else if (contains(str[i], "\t\n\r\v\f "))
			len--;
		else
			found_space = 0;
	}
	return (len);
}

char	*handle_var_spaces(char *str, char *fullcmd, int count)
{
	char	*new;
	char	*tmp;
	size_t	i;
	size_t	len;

	tmp = ft_strdup(str);
	if (!tmp)
		return (NULL);
	len = len_no_spaces(str);
	new = malloc(sizeof(char) * (len + 3));
	if (!new)
		return (free(tmp), NULL);
	i = ft_strlen(str);
	if (str && i > 0)
		while ((i - 1) > 0 && str[i - 1] && contains(str[i - 1], "\t\n\r\v\f "))
			i--;
	str[i] = 0;
	i = 0;
	while (str[i] && contains(str[i], "\t\n\r\v\f "))
		i++;
	form_new(&new, str, i);
	return (add_spaces(tmp, fullcmd, count, new));
}

int mid_space(char *str)
{
	int	i;
	
	i = 0;
	while (str[i] == ' ')
		i++;
	while (str[i] && str[i] != ' ')
		i++;
	while (str[i] == ' ')
	{
		i++;
		if (str[i] && str[i] != ' ')
			return (1);
	}
	return (0);
}

void error_undef(char *name)
{
	ft_putstr_fd("minishell: $", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd(": undefined redirect\n", 2);
	g_end_status = 1;
}

void	var_redir_undef(t_parse *elem, int space)
{
	elem->var_val = get_env_val(elem->p_data,elem->var);
	if (space == 1 && elem->var_val)
		elem->var_val = handle_var_spaces(elem->var_val, elem->fullcmd, elem->i);
	if (elem->var_val && space == 1 && elem->fullcmd[elem->i] != '$' && (only_spaces(elem->var_val) || mid_space(elem->var_val)))
		error_undef(elem->var);
	if (!elem->var_val && space == 1 && elem->fullcmd[elem->i] != '$')
		error_undef(elem->var);
	free(elem->var);
}

int	var_state(t_parse *elem, int isarg, int i)
{
	int		j;

	j = elem->i;
	if (isarg >= 2 && j != (int)ft_strlen(elem->fullcmd))
		j += 1;
	if (elem->fullcmd[j] && (j < (int)ft_strlen(elem->fullcmd)) && elem->fullcmd[j] == '$' && elem->fullcmd[j + 1] == '?')
	{
		if (j == 0 || (j > 0 && elem->fullcmd[j - 1] != '\\' && elem->fullcmd[j - 1] != '$'))
		{
			elem->var_val = ft_itoa(g_end_status);
			place_var(elem, isarg, i);
			if (elem->var_val)
				free(elem->var_val);
			elem->i++;
			return (1);
		}
	}
	return (0);
}

int var_redir(t_parse *elem, int dir, int space)
{
	int count;

	if (init_var_str(elem) == FAILURE)
		return (-1);
	elem->var[0] = 0;
	elem->i++;
	if (elem->fullcmd[elem->i] == '\'' || elem->fullcmd[elem->i] == '"')
	{
		free(elem->var);
		return (0);
	}
	if (var_state(elem, dir + 2, 0) == 1)
		return (1);
	find_var(elem, 1);
	var_redir_undef(elem, space);
	count = test_value(elem);
	if (count != 0)
	{
		if (elem->var_val)
			free(elem->var_val);
		return (count);
	}
	place_var(elem, dir + 2, 0);
	return (free(elem->var_val), elem->i--, -1);
}

int	var_handler(t_parse *elem, int isarg, int nb, int keep_space)
{
	if (dollar_error(elem, isarg, nb) != SUCCESS || var_state(elem, isarg, nb) == 1)
		return (1);
	if (init_var_str(elem) == FAILURE)
		return (-1);
	if (find_var(elem, 0) != SUCCESS)
		return (free(elem->var), 0);
	elem->var_val = get_env_val(elem->p_data,elem->var);
	if (keep_space == 1 && elem->var_val)
		elem->var_val = handle_var_spaces(elem->var_val, elem->fullcmd, elem->i);
	keep_space = test_value(elem);
	if (keep_space != 0)
	{
		if (elem->var_val)
			free (elem->var_val);
		return (keep_space);
	}
	place_var(elem, isarg, nb);
	free(elem->var_val);
	elem->i--;
	return (1);
}
