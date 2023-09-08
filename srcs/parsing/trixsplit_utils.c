/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trixsplit_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 17:12:41 by lmorel            #+#    #+#             */
/*   Updated: 2023/09/08 17:27:45 by ibenhaim         ###   ########.fr       */
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

int	watch_protect(char const *s, int i, char c, t_trix *split)
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

void	put_words_util(char const *s, t_trix *sp)
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

void	put_words(char const *s, int words, t_trix *sp)
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
