/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibenhaim <ibenhaim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 15:07:02 by ibenhaim          #+#    #+#             */
/*   Updated: 2023/07/26 10:28:44 by ibenhaim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "./minishell.h"

t_env	*get_env(t_data *data);
int		is_builtin(char *cmd, t_data *data);
void	built_exit(t_data *data, char **args);
int		built_echo(char **args);
int		built_pwd(void);
int		built_cd(char **args);
int		built_env(t_data *data);
void	print_env(t_env *env);
int		built_export(t_data *data);
void	sort_and_print(t_data *data);

#endif