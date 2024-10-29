/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands_1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoeller <tmoeller@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:57:36 by tmoeller          #+#    #+#             */
/*   Updated: 2024/10/22 13:04:40 by tmoeller         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (args[i] && ft_strcmp(args[i], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}

void	execute_cd(t_env **env_list, char **args)
{
	static char	prev_dir[PATH_MAX] = "";
	char		cwd[PATH_MAX];
	char		*path;

	if (args[2])
	{
		ft_fprintf(stderr, "minishell: cd: too many arguments\n");
		return ;
	}
	path = get_cd_path(env_list, args, prev_dir);
	if (!path)
		return ;
	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		perror("minishell: cd");
		return ;
	}
	change_directory(env_list, path, cwd, prev_dir);
}

void	execute_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX))
		printf("%s\n", cwd);
	else
		perror("minishell: pwd");
}

void	execute_env(t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		printf("%s=%s\n", current->name, current->value);
		current = current->next;
	}
}

void	execute_exit(t_env *env_list, char **args)
{
	int	exit_status;

	exit_status = 0;
	if (args[1])
	{
		validate_exit_arg(args[1], env_list);
		exit_status = ft_atoi(args[1]);
	}
	free_env_list(env_list);
	exit(exit_status);
}
