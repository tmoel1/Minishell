/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_and_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoeller <tmoeller@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:01:27 by tmoeller          #+#    #+#             */
/*   Updated: 2024/10/22 13:09:48 by tmoeller         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**allocate_result(size_t count)
{
	char	**result;

	result = malloc((count + 1) * sizeof(char *));
	if (!result)
	{
		perror("minishell: malloc");
		return (NULL);
	}
	return (result);
}

size_t	count_words(const char *s, char c)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (count);
}

int	fill_result_array(const char *s, char c, char **result)
{
	size_t	i;
	size_t	j;
	size_t	start;
	char	*word;

	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		if (i > start)
		{
			word = ft_strndup(s + start, i - start);
			if (!word)
				return (0);
			result[j++] = word;
		}
	}
	result[j] = NULL;
	return (1);
}

void	free_result(char **result)
{
	size_t	i;

	i = 0;
	while (result[i])
	{
		free(result[i]);
		i++;
	}
	free(result);
}

char	**ft_split(const char *s, char c)
{
	size_t	count;
	char	**result;

	if (!s)
		return (NULL);
	count = count_words(s, c);
	result = allocate_result(count);
	if (!result)
		return (NULL);
	if (!fill_result_array(s, c, result))
	{
		free_result(result);
		return (NULL);
	}
	return (result);
}
