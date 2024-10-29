/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoeller <tmoeller@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:01:01 by tmoeller          #+#    #+#             */
/*   Updated: 2024/10/22 13:09:41 by tmoeller         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	error_missing_argument(void)
{
	ft_fprintf(stderr,
		"minishell: syntax error near unexpected token `newline'\n");
	exit(258);
}

void	remove_redirection_tokens(char **args, int i)
{
	int	j;

	free(args[i]);
	free(args[i + 1]);
	j = i;
	while (args[j + 2])
	{
		args[j] = args[j + 2];
		j++;
	}
	args[j] = NULL;
}

void	read_heredoc(char *delimiter, int write_fd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
}

void	update_quote_status(char c, int *in_s_quote, int *in_d_quote)
{
	if (c == '\'' && !(*in_d_quote))
		*in_s_quote = !(*in_s_quote);
	else if (c == '\"' && !(*in_s_quote))
		*in_d_quote = !(*in_d_quote);
}
