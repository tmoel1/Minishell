/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_variables.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astoll <astoll@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:24:55 by astoll            #+#    #+#             */
/*   Updated: 2024/10/22 13:24:57 by astoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_env_variable(t_env **env_list, char *name, char *value)
{
	t_env	*new_node;
	t_env	*current;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		perror("minishell: malloc");
		exit(1);
	}
	new_node->name = ft_strdup(name);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	if (!(*env_list))
		*env_list = new_node;
	else
	{
		current = *env_list;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}

void	load_env(t_env **env_list, char **envp)
{
	char	*equal_sign;
	char	*name;
	char	*value;
	int		i;

	i = 0;
	while (envp[i])
	{
		equal_sign = ft_strchr(envp[i], '=');
		if (equal_sign)
		{
			name = ft_strndup(envp[i], equal_sign - envp[i]);
			value = ft_strdup(equal_sign + 1);
			add_env_variable(env_list, name, value);
			free(name);
			free(value);
		}
		i++;
	}
}

char	*get_env_value(t_env *env_list, char *name)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	set_env_value(t_env **env_list, char *name, char *value)
{
	t_env	*current;

	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	add_env_variable(env_list, name, value);
}

void	unset_env_variable(t_env **env_list, char *name)
{
	t_env	*current;
	t_env	*prev;

	current = *env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env_list = current->next;
			free(current->name);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}
