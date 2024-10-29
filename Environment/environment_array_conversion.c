/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_array_conversion.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astoll <astoll@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:24:38 by astoll            #+#    #+#             */
/*   Updated: 2024/10/22 13:24:44 by astoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*create_env_string(char *name, char *value)
{
	char	*env_string;
	size_t	len;

	len = ft_strlen(name) + ft_strlen(value) + 2;
	env_string = malloc(len);
	if (!env_string)
	{
		perror("minishell: malloc");
		exit(1);
	}
	ft_strcpy(env_string, name);
	ft_strcat(env_string, "=");
	ft_strcat(env_string, value);
	return (env_string);
}
