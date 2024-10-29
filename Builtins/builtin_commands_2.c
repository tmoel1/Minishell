/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoeller <tmoeller@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:57:57 by tmoeller          #+#    #+#             */
/*   Updated: 2024/10/22 13:05:48 by tmoeller         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_unset(t_env **env_list, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		unset_env_variable(env_list, args[i]);
		i++;
	}
}

void	execute_export(t_env **env_list, char **args)
{
	int	i;

	if (!args[1])
	{
		print_exported_variables(*env_list);
		return ;
	}
	i = 1;
	while (args[i])
	{
		export_variable(env_list, args[i]);
		i++;
	}
}
