/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:07:02 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/08/14 12:47:13 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "./minishell.h"

t_env	*get_env(t_data *data);
int		is_builtin(char *cmd, t_data *data);
void	built_exit(t_data *data, char **args);
int		built_echo(t_data *data, char **args);
int		built_pwd(t_data *data);
int		built_cd(char **args);
int		built_env(t_data *data);
void	print_env(t_env *env, int std);
int		built_export(t_data *data);
int		sort_and_print(t_env *lst);
void	ft_lstadd_back(t_env **lst, t_env *new);
t_env	*new_env(char *line);
int		built_unset(t_data *data);
void	free_all_env(t_data *data);
int		is_alpha(char c);

#endif