/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astoll <astoll@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:26:06 by astoll            #+#    #+#             */
/*   Updated: 2024/10/22 13:26:08 by astoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	tokenize_input(t_env *env_list, char *input, char **tokens)
{
	t_tokenize_vars	v;

	initialize_tokenize_vars(&v);
	tokenize_input_loop(env_list, input, tokens, &v);
	tokens[v.token_index] = NULL;
}

void	initialize_tokenize_vars(t_tokenize_vars *v)
{
	v->i = 0;
	v->token_index = 0;
	v->vars[0] = 0;
	v->vars[1] = 0;
	v->vars[2] = 0;
	v->vars[3] = 0;
}

void	tokenize_input_loop(t_env *env_list, char *input,
			char **tokens, t_tokenize_vars *v)
{
	while (input[v->i])
	{
		skip_whitespace(input, &(v->i));
		if (!input[v->i])
			break ;
		if (v->token_index >= MAX_TOKENS - 1)
		{
			handle_too_many_tokens(tokens, v->token_index);
			return ;
		}
		if (!is_special_sequence(input[v->i]))
		{
			v->vars[0] = v->i;
			v->vars[1] = 0;
			process_regular_token(env_list, input, v->token, v->vars);
			v->i = v->vars[0];
			add_token_to_list(tokens, &(v->token_index), v->token);
		}
		else
			handle_special_token(input, &(v->i), tokens, &(v->token_index));
	}
}

void	handle_too_many_tokens(char **tokens, int token_index)
{
	int	j;

	ft_fprintf(stderr, "minishell: error: too many tokens in command\n");
	j = 0;
	while (j < token_index)
	{
		free(tokens[j]);
		j++;
	}
	tokens[0] = NULL;
}
