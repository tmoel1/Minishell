/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands_helpers_1.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoeller <tmoeller@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:58:15 by tmoeller          #+#    #+#             */
/*   Updated: 2024/10/22 13:06:06 by tmoeller         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_cd_path(t_env **env_list, char **args, char *prev_dir)
{
	char	*path;
	char	*home;

	path = args[1];
	home = get_env_value(*env_list, "HOME");
	if (!path)
		path = home;
	else if (ft_strcmp(path, "-") == 0)
	{
		if (prev_dir[0] == '\0')
		{
			ft_fprintf(stderr, "minishell: cd: OLDPWD not set\n");
			return (NULL);
		}
		printf("%s\n", prev_dir);
		path = prev_dir;
	}
	if (!path)
	{
		ft_fprintf(stderr, "minishell: cd: HOME not set\n");
		return (NULL);
	}
	return (path);
}

void	update_pwd(t_env **env_list, char *cwd, char *prev_dir)
{
	set_env_value(env_list, "OLDPWD", cwd);
	ft_strcpy(prev_dir, cwd);
	if (getcwd(cwd, PATH_MAX))
		set_env_value(env_list, "PWD", cwd);
}

void	change_directory(t_env **env_list, char *path, char *cwd,
			char *prev_dir)
{
	if (chdir(path) != 0)
		perror("minishell: cd");
	else
		update_pwd(env_list, cwd, prev_dir);
}

void	validate_exit_arg(char *arg, t_env *env_list)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (!isdigit(arg[i]) && !(i == 0 && arg[i] == '-'))
		{
			ft_fprintf(stderr, "minishell: exit: numeric argument required\n");
			free_env_list(env_list);
			exit(255);
		}
		i++;
	}
}

void	export_variable(t_env **env_list, char *arg)
{
	char	*equal_sign;
	char	*name;
	char	*value;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		name = ft_strndup(arg, equal_sign - arg);
		value = ft_strdup(equal_sign + 1);
		set_env_value(env_list, name, value);
		free(name);
		free(value);
	}
	else
		set_env_value(env_list, arg, "");
}
