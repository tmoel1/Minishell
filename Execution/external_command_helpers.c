/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_command_helpers.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoeller <tmoeller@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:59:29 by tmoeller          #+#    #+#             */
/*   Updated: 2024/10/22 13:11:47 by tmoeller         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_non_builtin_command(t_env **env_list, char **args)
{
	char	**envp_array;
	int		j;

	j = 0;
	envp_array = env_list_to_envp(*env_list);
	execute_external(*env_list, args, envp_array);
	while (envp_array[j])
		free(envp_array[j++]);
	free(envp_array);
}

char	**get_path_dirs(t_env *env_list)
{
	char	*path;
	char	**path_dirs;

	path = get_env_value(env_list, "PATH");
	if (!path)
	{
		ft_fprintf(stderr, "minishell: command not found\n");
		exit(127);
	}
	path_dirs = ft_split(path, ':');
	if (!path_dirs)
	{
		perror("minishell: malloc");
		exit(1);
	}
	return (path_dirs);
}

void	search_and_execute(char **args, char **envp, char **path_dirs)
{
	char	full_path[PATH_MAX];
	int		i;

	i = 0;
	while (path_dirs[i])
	{
		ft_strcpy(full_path, path_dirs[i]);
		ft_strcat(full_path, "/");
		ft_strcat(full_path, args[0]);
		if (access(full_path, X_OK) == 0)
		{
			execve(full_path, args, envp);
			perror("minishell");
			free_split(path_dirs);
			exit(127);
		}
		i++;
	}
}

void	command_not_found(char **args, char **path_dirs)
{
	ft_fprintf(stderr, "minishell: command not found: %s\n", args[0]);
	free_split(path_dirs);
	exit(127);
}
