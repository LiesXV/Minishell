/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trixsplit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorel <lmorel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 23:18:22 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/03 01:05:58 by lmorel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_all_split(char **str, int index)
{
	while (index)
	{
		free(str[index]);
		index--;
	}
	free(str);
}

int				watch_protect(char const *s, int i, char c, t_trix *split)
{
	i++;
	while (s[i] && s[i] != c)
	{
		if (s[i] == '\\')
		{
			i++;
			split->k++;
		}
		split->k++;
		i++;
	}
	if (i == (int)ft_strlen(s))
		return (i);
	i++;
	split->k = split->k + 2;
	return (i);
}

void			put_words_util(char const *s, t_trix *sp)
{
	char	quote;

	while (s[sp->j] && (s[sp->j] == '\'' || s[sp->j] == '"'))
	{
		quote = s[sp->j];
		sp->str[sp->i][sp->k++] = s[sp->j++];
		while (s[sp->j] && s[sp->j] != quote)
		{
			if (s[sp->j] == '\\')
				sp->str[sp->i][sp->k++] = s[sp->j++];
			if (sp->j == (int)ft_strlen(s))
				return ;
			sp->str[sp->i][sp->k++] = s[sp->j++];
		}
		if (sp->j == (int)ft_strlen(s))
			return ;
		sp->str[sp->i][sp->k++] = s[sp->j++];
	}
	if (s[sp->j] == '\\')
		sp->str[sp->i][sp->k++] = s[sp->j++];
}

void			put_words(char const *s, int words, t_trix *sp)
{
	sp->i = -1;
	sp->j = 0;
	while (s && s[sp->j] && ++sp->i < words)
	{
		sp->k = 0;
		while (s[sp->j] && s[sp->j] == sp->c)
			sp->j++;
		while ((s[sp->j] && (s[sp->j] != sp->c
			|| (s[sp->j] == sp->c && sp->j > 0
			&& s[sp->j - 1] == '\\'))))
		{
			put_words_util(s, sp);
			if (sp->j && s[sp->j - 1] != '\\' && (s[sp->j] == sp->c
				|| s[sp->j] == '\0'))
				break ;
			if (sp->j == (int)ft_strlen(s))
				break ;
			sp->str[sp->i][sp->k++] = s[sp->j++];
		}
		sp->str[sp->i][sp->k] = '\0';
	}
}

int			count_words_util(char const *s, t_trix *sp)
{
	while (s[sp->i] && (s[sp->i] == '\'' || s[sp->i] == '"'))
	{
		while (s[sp->i] && s[sp->i] == '"')
			sp->i = watch_protect(s, sp->i, '"', sp);
		while (s[sp->i] && s[sp->i] == '\'')
			sp->i = watch_protect(s, sp->i, '\'', sp);
	}
	if (sp->i == sp->len)
		return (-1);
	if (s[sp->i] == '\\')
		sp->i++;
	return (0);
}

static int	count_words(char const *s, char c, t_trix *sp)
{
	sp->i = -1;
	sp->len = (int)ft_strlen(s);
	sp->count_words = 0;
	while (s[++sp->i])
	{
		if (count_words_util(s, sp) == -1)
			break ;
		if (s[sp->i] && s[sp->i] == c && s[sp->i + 1] != c &&
			s[sp->i - 1] != '\\')
		{
			sp->j = sp->i + 1;
			while (s[sp->j] && sp->j == ' ')
				sp->j++;
			if (sp->j != (int)ft_strlen(s))
				sp->count_words++;
		}
		if (sp->i == sp->len)
			break ;
	}
	sp->count_words++;
	return (sp->count_words);
}

int			malloc_words_util(char const *s, char c, t_trix *sp)
{
	while (s[sp->j] == '\'' || s[sp->j] == '"')
	{
		while (s[sp->j] == '"')
			sp->j = watch_protect(s, sp->j, '"', sp);
		while (s[sp->j] == '\'')
			sp->j = watch_protect(s, sp->j, '\'', sp);
	}
	if (s[sp->j] && s[sp->j] == '\\')
	{
		sp->j++;
		sp->k++;
	}
	if ((s[sp->j] && s[sp->j] == '\0')
		|| (s[sp->j] == c && s[sp->j - 1] != '\\'))
		return (-1);
	return (0);
}

int			malloc_words(char const *s, char **str,
			int words, t_trix *sp)
{
	sp->i = -1;
	sp->j = -1;
	while (++sp->i < words)
	{
		sp->k = 0;
		while (s[++sp->j] && (s[sp->j] != sp->c || (s[sp->j] == sp->c
			&& (sp->j == 0 || s[sp->j - 1] == '\\'))))
		{
			if (malloc_words_util(s, sp->c, sp) == -1)
				break ;
			sp->k++;
			if (sp->j == (int)ft_strlen(s))
				break ;
		}
		while (s[sp->j] && s[sp->j] == sp->c)
			sp->j++;
		sp->j--;
		str[sp->i] = (char *)malloc(sizeof(char) * (sp->k + 1));
		if (!str[sp->i])
			return (free_all_split(str, sp->i), 0);
	}
	return (1);
}

char		**trixsplit(char const *s, char c)
{
	char	**str;
	int		nb_words;
	t_trix	split;

	nb_words = 0;
	split.c = c;
	if (!s)
		return (NULL);
	nb_words = count_words(s, c, &split);
	str = (char **)malloc(sizeof(char *) * (nb_words + 1)); //leaks
	if (!str)
		return (NULL);
	if (!malloc_words(s, str, nb_words, &split))
		return (NULL);
	split.str = str;
	put_words(s, nb_words, &split);
	str[nb_words] = 0;
	return (str);
}
