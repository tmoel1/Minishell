/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astoll <astoll@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:25:33 by astoll            #+#    #+#             */
/*   Updated: 2024/10/22 13:25:35 by astoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile sig_atomic_t	g_received_signal = 0;

int	main(int argc, char **argv, char **envp)
{
	t_env	*env_list;

	(void)argc;
	(void)argv;
	env_list = NULL;
	initialize_shell();
	load_env(&env_list, envp);
	main_loop(&env_list);
	free_env_list(env_list);
	return (0);
}

void	main_loop(t_env **env_list)
{
	char	*input;

	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		process_input_line(env_list, input);
		free(input);
	}
}

void	process_input_line(t_env **env_list, char *input)
{
	if (ft_strlen(input) > MAX_CMD_LEN)
	{
		ft_fprintf(stderr, "minishell: error: input exceeds max allowed "
			"length of %d char\n", MAX_CMD_LEN);
		return ;
	}
	if (ft_strchr(input, '|'))
		execute_pipeline(env_list, input);
	else
		execute_command(env_list, input);
}
