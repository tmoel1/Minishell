/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_list_management.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astoll <astoll@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:24:49 by astoll            #+#    #+#             */
/*   Updated: 2024/10/22 13:24:51 by astoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_env_list(t_env *env_list)
{
	t_env	*current;
	t_env	*next;

	current = env_list;
	while (current)
	{
		next = current->next;
		free(current->name);
		free(current->value);
		free(current);
		current = next;
	}
}

int	count_env_vars(t_env *env_list)
{
	int		count;
	t_env	*current;

	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

char	**env_list_to_envp(t_env *env_list)
{
	int		count;
	char	**envp_array;

	count = count_env_vars(env_list);
	envp_array = allocate_envp_array(count);
	fill_envp_array(env_list, envp_array);
	return (envp_array);
}

char	**allocate_envp_array(int count)
{
	char	**envp_array;

	envp_array = malloc(sizeof(char *) * (count + 1));
	if (!envp_array)
	{
		perror("minishell: malloc");
		exit(1);
	}
	return (envp_array);
}

void	fill_envp_array(t_env *env_list, char **envp_array)
{
	t_env	*current;
	int		i;

	current = env_list;
	i = 0;
	while (current)
	{
		envp_array[i++] = create_env_string(current->name, current->value);
		current = current->next;
	}
	envp_array[i] = NULL;
}
