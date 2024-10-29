/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_splitting.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoeller <tmoeller@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:00:33 by tmoeller          #+#    #+#             */
/*   Updated: 2024/10/22 13:08:30 by tmoeller         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	split_pipeline(char *cmd_line, char **commands)
{
	t_pipeline_vars	vars;

	initialize_pipeline_vars(&vars);
	if (!parse_pipeline(cmd_line, commands, &vars))
		return (free_commands(commands, vars.cmd_index));
	if (!finalize_pipeline(cmd_line, commands, &vars))
		return (free_commands(commands, vars.cmd_index));
	commands[vars.cmd_index] = NULL;
	return (vars.cmd_index);
}

void	initialize_pipeline_vars(t_pipeline_vars *vars)
{
	vars->i = 0;
	vars->cmd_index = 0;
	vars->start = 0;
	vars->in_s_quote = 0;
	vars->in_d_quote = 0;
}

int	finalize_pipeline(char *cmd_line, char **commands, t_pipeline_vars *vars)
{
	cmd_line[vars->i] = '\0';
	vars->start = vars->start;
	if (!allocate_command(commands, &vars->cmd_index, cmd_line + vars->start))
		return (0);
	return (1);
}

int	parse_pipeline(char *cmd_line, char **commands, t_pipeline_vars *vars)
{
	while (cmd_line[vars->i])
	{
		update_quote_status(cmd_line[vars->i],
			&vars->in_s_quote, &vars->in_d_quote);
		if (cmd_line[vars->i] == '|' && !vars->in_s_quote && !vars->in_d_quote)
		{
			cmd_line[vars->i] = '\0';
			if (!process_pipeline_token(cmd_line, commands, vars))
				return (0);
			vars->start = vars->i + 1;
		}
		vars->i++;
	}
	return (1);
}
