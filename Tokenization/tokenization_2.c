/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astoll <astoll@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:26:11 by astoll            #+#    #+#             */
/*   Updated: 2024/10/22 13:26:12 by astoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	process_regular_token(t_env *env_list, char *input, char *token,
			int *vars)
{
	vars[2] = 0;
	vars[3] = 0;
	process_token_loop(env_list, input, token, vars);
	token[vars[1]] = '\0';
}

void	process_token_loop(t_env *env_list, char *input, char *token,
			int *vars)
{
	while (input[vars[0]])
	{
		if (input[vars[0]] == '\'' && !vars[3])
		{
			vars[2] = !vars[2];
			vars[0]++;
		}
		else if (input[vars[0]] == '\"' && !vars[2])
		{
			vars[3] = !vars[3];
			vars[0]++;
		}
		else if (!vars[2] && !vars[3] && (ft_isspace(input[vars[0]])
				|| is_special_sequence(input[vars[0]])))
			break ;
		else if (input[vars[0]] == '$' && !vars[2])
		{
			vars[0]++;
			expand_variable_in_token(env_list, input, token, vars);
		}
		else
			ft_strappend_char(token, input[vars[0]++], &(vars[1]), MAX_CMD_LEN);
	}
}

void	expand_variable_in_token(t_env *env_list, char *input, char *token,
			int *vars)
{
	char	var_name[256];
	int		k;
	char	*value;
	char	*num_str;

	k = 0;
	if (input[vars[0]] == '?')
	{
		num_str = ft_itoa(g_received_signal);
		ft_strappend(token, num_str, &(vars[1]), MAX_CMD_LEN);
		free(num_str);
		vars[0]++;
	}
	else if (ft_isalnum(input[vars[0]]) || input[vars[0]] == '_')
	{
		while (ft_isalnum(input[vars[0]]) || input[vars[0]] == '_')
			var_name[k++] = input[(vars[0])++];
		var_name[k] = '\0';
		value = get_env_value(env_list, var_name);
		if (value)
			ft_strappend(token, value, &(vars[1]), MAX_CMD_LEN);
	}
	else
		ft_strappend_char(token, '$', &(vars[1]), MAX_CMD_LEN);
}

void	handle_special_token(char *input, int *i, char **tokens,
			int *token_index)
{
	char	token[3];
	int		token_size;

	token_size = 0;
	if ((input[*i] == '>' || input[*i] == '<') && input[*i + 1] == input[*i])
		token[token_size++] = input[(*i)++];
	token[token_size++] = input[(*i)++];
	token[token_size] = '\0';
	tokens[(*token_index)++] = ft_strdup(token);
}
