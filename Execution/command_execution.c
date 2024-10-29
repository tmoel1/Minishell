/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoeller <tmoeller@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:59:03 by tmoeller          #+#    #+#             */
/*   Updated: 2024/10/22 13:11:37 by tmoeller         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_command(t_env **env_list, char *input)
{
	char	**args;

	args = malloc(sizeof(char *) * MAX_TOKENS);
	if (!args)
	{
		perror("minishell: malloc");
		exit(1);
	}
	tokenize_input(*env_list, input, args);
	if (args[0])
		process_command(env_list, args);
	free_tokens(args);
	free(args);
}

void	process_command(t_env **env_list, char **args)
{
	if (args[0][0] == '\0')
	{
		ft_fprintf(stderr, "minishell: command not found\n");
		return ;
	}
	if (is_builtin(args[0]))
	{
		execute_builtin(env_list, args);
		g_received_signal = 0;
	}
	else
		execute_external_command(env_list, args);
}

void	execute_builtin(t_env **env_list, char **args)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
	{
		perror("minishell: dup");
		return ;
	}
	handle_redirections(args);
	if (args[0] == NULL)
	{
		restore_std_fds(saved_stdin, saved_stdout);
		return ;
	}
	execute_specific_builtin(env_list, args);
	restore_std_fds(saved_stdin, saved_stdout);
}

int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0 || ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "export") == 0)
		return (1);
	return (0);
}

void	execute_specific_builtin(t_env **env_list, char **args)
{
	if (ft_strcmp(args[0], "echo") == 0)
		execute_echo(args);
	else if (ft_strcmp(args[0], "cd") == 0)
		execute_cd(env_list, args);
	else if (ft_strcmp(args[0], "pwd") == 0)
		execute_pwd();
	else if (ft_strcmp(args[0], "env") == 0)
		execute_env(*env_list);
	else if (ft_strcmp(args[0], "exit") == 0)
		execute_exit(*env_list, args);
	else if (ft_strcmp(args[0], "unset") == 0)
		execute_unset(env_list, args);
	else if (ft_strcmp(args[0], "export") == 0)
		execute_export(env_list, args);
}
