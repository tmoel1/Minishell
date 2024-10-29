/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_command_execution.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoeller <tmoeller@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:59:16 by tmoeller          #+#    #+#             */
/*   Updated: 2024/10/22 13:11:42 by tmoeller         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_external_command(t_env **env_list, char **args)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
		execute_child_process_main(env_list, args);
	else if (pid < 0)
		perror("minishell: fork");
	else
	{
		waitpid(pid, &status, 0);
		handle_child_exit_status(status);
	}
}

void	execute_child_process_main(t_env **env_list, char **args)
{
	int		i;
	char	**envp_array;

	handle_redirections(args);
	envp_array = env_list_to_envp(*env_list);
	execute_external(*env_list, args, envp_array);
	i = 0;
	while (envp_array[i])
		free(envp_array[i++]);
	free(envp_array);
	exit(1);
}

void	handle_child_exit_status(int status)
{
	if (WIFEXITED(status))
		g_received_signal = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_received_signal = 128 + WTERMSIG(status);
}

void	execute_external(t_env *env_list, char **args, char **envp)
{
	char	**path_dirs;

	if (args[0] == NULL || args[0][0] == '\0')
	{
		ft_fprintf(stderr, "minishell: command not found\n");
		exit(127);
	}
	try_execute_direct(args, envp);
	path_dirs = get_path_dirs(env_list);
	search_and_execute(args, envp, path_dirs);
	command_not_found(args, path_dirs);
}

void	try_execute_direct(char **args, char **envp)
{
	if (args[0][0] == '/' || args[0][0] == '.')
	{
		if (access(args[0], X_OK) == 0)
		{
			execve(args[0], args, envp);
			perror("minishell");
			exit(127);
		}
	}
}
