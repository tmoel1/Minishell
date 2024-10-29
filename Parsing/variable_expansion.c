/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astoll <astoll@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:25:51 by astoll            #+#    #+#             */
/*   Updated: 2024/10/22 13:25:52 by astoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	process_variable_expansion(t_env *env_list, char *input, char *result,
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
		ft_strappend(result, num_str, &(vars[1]), MAX_CMD_LEN);
		free(num_str);
		vars[0]++;
	}
	else if (ft_isalnum(input[vars[0]]) || input[vars[0]] == '_')
	{
		handle_variable_char(input, &(vars[0]), var_name, &k);
		var_name[k] = '\0';
		value = get_variable_value(env_list, var_name);
		ft_strappend(result, value, &(vars[1]), MAX_CMD_LEN);
		free(value);
	}
	else
		ft_strappend_char(result, '$', &(vars[1]), MAX_CMD_LEN);
}

void	process_other_character(char *input, char *result, int *vars)
{
	ft_strappend_char(result, input[(vars[0])++], &(vars[1]), MAX_CMD_LEN);
}

char	*get_variable_value(t_env *env_list, char *var_name)
{
	char	*value;

	value = get_env_value(env_list, var_name);
	if (value)
		return (ft_strdup(value));
	else
		return (ft_strdup(""));
}

void	handle_variable_char(char *input, int *i, char *var_name, int *k)
{
	while (ft_isalnum(input[*i]) || input[*i] == '_')
		var_name[(*k)++] = input[(*i)++];
}

void	ft_strappend(char *dest, const char *src, int *j, int max_len)
{
	int	i;

	i = 0;
	while (src[i] && *j < max_len - 1)
		dest[(*j)++] = src[i++];
}
