/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handling.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoeller <tmoeller@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:00:47 by tmoeller          #+#    #+#             */
/*   Updated: 2024/10/22 13:09:37 by tmoeller         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_redirections(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "<") == 0)
			handle_input_redirection(args, &i);
		else if (ft_strcmp(args[i], ">") == 0)
			handle_output_redirection(args, &i);
		else if (ft_strcmp(args[i], ">>") == 0)
			handle_append_redirection(args, &i);
		else if (ft_strcmp(args[i], "<<") == 0)
			handle_heredoc_redirection(args, &i);
		else
			i++;
	}
}

void	handle_input_redirection(char **args, int *i)
{
	int	fd;

	if (!args[*i + 1])
		error_missing_argument();
	fd = open(args[*i + 1], O_RDONLY);
	if (fd < 0)
	{
		perror("minishell");
		exit(1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	remove_redirection_tokens(args, *i);
}

void	handle_output_redirection(char **args, int *i)
{
	int	fd;

	if (!args[*i + 1])
		error_missing_argument();
	fd = open(args[*i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("minishell");
		exit(1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	remove_redirection_tokens(args, *i);
}

void	handle_append_redirection(char **args, int *i)
{
	int	fd;

	if (!args[*i + 1])
		error_missing_argument();
	fd = open(args[*i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("minishell");
		exit(1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	remove_redirection_tokens(args, *i);
}

void	handle_heredoc_redirection(char **args, int *i)
{
	int	fds[2];

	if (!args[*i + 1])
		error_missing_argument();
	if (pipe(fds) == -1)
	{
		perror("minishell: pipe");
		exit(1);
	}
	read_heredoc(args[*i + 1], fds[1]);
	close(fds[1]);
	dup2(fds[0], STDIN_FILENO);
	close(fds[0]);
	remove_redirection_tokens(args, *i);
}
