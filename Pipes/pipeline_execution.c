/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_execution.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoeller <tmoeller@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:00:09 by tmoeller          #+#    #+#             */
/*   Updated: 2024/10/22 13:08:21 by tmoeller         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_pipeline(t_env **env_list, char *cmd_line)
{
	char	*commands[MAX_TOKENS];
	int		num_commands;

	num_commands = split_pipeline(cmd_line, commands);
	if (num_commands == -1)
		return ;
	execute_command_loop(env_list, commands, num_commands);
}

void	execute_command_loop(t_env **env_list, char **commands,
			int num_commands)
{
	int		i;
	int		prev_fd;
	pid_t	pids[MAX_TOKENS];
	int		fd[2];
	pid_t	pid;

	prev_fd = -1;
	i = 0;
	while (i < num_commands)
	{
		setup_pipes(i, num_commands, fd);
		pid = fork_and_execute(env_list, commands[i], fd, prev_fd);
		if (pid < 0)
		{
			perror("minishell: fork");
			exit(1);
		}
		pids[i] = pid;
		execute_parent_process(fd, &prev_fd);
		i++;
	}
	if (prev_fd != -1)
		close(prev_fd);
	wait_for_children(num_commands, pids, commands);
}

pid_t	fork_and_execute(t_env **env_list, char *command, int fd[2],
			int prev_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		execute_child_process(env_list, command, fd, prev_fd);
		exit(0);
	}
	return (pid);
}

void	execute_child_process(t_env **env_list, char *command, int fd[2],
			int prev_fd)
{
	char	**args;

	setup_child_redirections(prev_fd, fd);
	allocate_and_tokenize_args(*env_list, command, &args);
	handle_redirections(args);
	execute_command_in_child(env_list, args);
}

void	execute_parent_process(int fd[2], int *prev_fd)
{
	if (fd[1] != -1)
		close(fd[1]);
	if (*prev_fd != -1)
		close(*prev_fd);
	*prev_fd = fd[0];
}
