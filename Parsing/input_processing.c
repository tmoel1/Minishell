/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_processing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astoll <astoll@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:25:46 by astoll            #+#    #+#             */
/*   Updated: 2024/10/22 13:25:47 by astoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expand_variables(t_env *env_list, char *input)
{
	char	*result;

	result = malloc(MAX_CMD_LEN);
	if (!result)
	{
		perror("minishell: malloc");
		exit(1);
	}
	process_input(env_list, input, result);
	return (result);
}

void	process_input(t_env *env_list, char *input, char *result)
{
	int	vars[4];

	vars[0] = 0;
	vars[1] = 0;
	vars[2] = 0;
	vars[3] = 0;
	process_input_loop(env_list, input, result, vars);
	result[vars[1]] = '\0';
}

void	process_input_loop(t_env *env_list, char *input, char *result,
			int *vars)
{
	while (input[vars[0]] && vars[1] < MAX_CMD_LEN - 1)
	{
		if (input[vars[0]] == '\'' && !vars[3])
			process_single_quote(input, result, vars, 2);
		else if (input[vars[0]] == '\"' && !vars[2])
			process_double_quote(input, result, vars, 3);
		else if (input[vars[0]] == '$' && !vars[2])
		{
			vars[0]++;
			process_variable_expansion(env_list, input, result, vars);
		}
		else
			process_other_character(input, result, vars);
	}
}

void	process_single_quote(char *input, char *result, int *vars,
			int quote_index)
{
	vars[quote_index] = !(vars[quote_index]);
	result[(vars[1])++] = input[(vars[0])++];
}

void	process_double_quote(char *input, char *result, int *vars,
			int quote_index)
{
	vars[quote_index] = !(vars[quote_index]);
	result[(vars[1])++] = input[(vars[0])++];
}
