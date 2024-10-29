/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility_functions_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astoll <astoll@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:26:31 by astoll            #+#    #+#             */
/*   Updated: 2024/10/22 13:26:33 by astoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strdup(const char *s1)
{
	char	*copy;
	size_t	len;

	len = ft_strlen(s1) + 1;
	copy = malloc(len);
	if (copy)
		ft_memcpy(copy, s1, len);
	return (copy);
}

char	*ft_strndup(const char *s1, size_t n)
{
	char	*copy;
	size_t	len;

	len = ft_strnlen(s1, n);
	copy = malloc(len + 1);
	if (copy)
	{
		ft_memcpy(copy, s1, len);
		copy[len] = '\0';
	}
	return (copy);
}

size_t	ft_strlen(const char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = 0;
	while (s[len])
		len++;
	return (len);
}

size_t	ft_strnlen(const char *s, size_t maxlen)
{
	size_t	len;

	len = 0;
	while (len < maxlen && s[len])
		len++;
	return (len);
}

void	free_split(char **strs)
{
	int	i;

	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}
