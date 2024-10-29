/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_execution_helpers.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoeller <tmoeller@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:59:57 by tmoeller          #+#    #+#             */
/*   Updated: 2024/10/22 13:08:16 by tmoeller         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	wait_for_children(int num_commands, pid_t *pids, char **commands)
{
	int	i;
	int	status;

	i = 0;
	while (i < num_commands)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			g_received_signal = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_received_signal = 128 + WTERMSIG(status);
		free(commands[i]);
		i++;
	}
}

void	setup_pipes(int i, int num_commands, int fd[2])
{
	if (i < num_commands - 1)
	{
		if (pipe(fd) == -1)
		{
			perror("minishell: pipe");
			exit(1);
		}
	}
	else
	{
		fd[0] = -1;
		fd[1] = -1;
	}
}

void	setup_child_redirections(int prev_fd, int fd[2])
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (fd[1] != -1)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	if (fd[0] != -1)
		close(fd[0]);
}

void	allocate_and_tokenize_args(t_env *env_list, char *command, char ***args)
{
	*args = malloc(sizeof(char *) * MAX_TOKENS);
	if (!(*args))
	{
		perror("minishell: malloc");
		exit(1);
	}
	tokenize_input(env_list, command, *args);
}

void	execute_command_in_child(t_env **env_list, char **args)
{
	if (args[0])
	{
		if (is_builtin(args[0]))
		{
			execute_builtin(env_list, args);
			free_tokens(args);
			free(args);
			exit(g_received_signal);
		}
		else
		{
			execute_non_builtin_command(env_list, args);
		}
	}
	free_tokens(args);
	free(args);
	exit(0);
}
