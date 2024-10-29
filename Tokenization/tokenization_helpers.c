/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astoll <astoll@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:26:16 by astoll            #+#    #+#             */
/*   Updated: 2024/10/22 13:26:18 by astoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_token_to_list(char **tokens, int *token_index, char *token)
{
	tokens[(*token_index)++] = ft_strdup(token);
}

void	skip_whitespace(char *input, int *i)
{
	while (ft_isspace(input[*i]))
		(*i)++;
}

int	is_special_sequence(char c)
{
	return (c == '>' || c == '<' || c == '|');
}

void	ft_strappend_char(char *dest, char c, int *j, int max_len)
{
	if (*j < max_len - 1)
		dest[(*j)++] = c;
}

void	free_tokens(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
}
