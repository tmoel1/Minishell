/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_splitting_helpers.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoeller <tmoeller@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:00:19 by tmoeller          #+#    #+#             */
/*   Updated: 2024/10/22 13:08:25 by tmoeller         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	process_pipeline_token(char *cmd_line, char **commands,
			t_pipeline_vars *vars)
{
	if (!allocate_command(commands, &vars->cmd_index, cmd_line + vars->start))
		return (0);
	if (!check_max_commands(vars->cmd_index))
		return (0);
	return (1);
}

int	allocate_command(char **commands, int *cmd_index, char *command)
{
	commands[*cmd_index] = ft_strdup(command);
	if (!commands[*cmd_index])
	{
		ft_fprintf(stderr, "minishell: error: memory allocation failed\n");
		return (0);
	}
	(*cmd_index)++;
	return (1);
}

int	check_max_commands(int cmd_index)
{
	if (cmd_index >= MAX_TOKENS - 1)
	{
		ft_fprintf(stderr, "minishell: error: too many commands in pipeline\n");
		return (0);
	}
	return (1);
}

int	free_commands(char **commands, int cmd_index)
{
	int	j;

	j = 0;
	while (j < cmd_index)
	{
		free(commands[j]);
		j++;
	}
	commands[0] = NULL;
	return (-1);
}
