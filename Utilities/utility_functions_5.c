/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility_functions_5.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astoll <astoll@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:26:50 by astoll            #+#    #+#             */
/*   Updated: 2024/10/22 13:26:52 by astoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_itoa(int nbr)
{
	int		len;
	long	nb;
	char	*str;

	len = ft_len(nbr);
	nb = nbr;
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (0);
	if (nb < 0)
	{
		nb = -nb;
		str[0] = '-';
	}
	str[len] = '\0';
	str[len - 1] = '0';
	while (nb > 0)
	{
		len--;
		str[len] = (nb % 10) + 48;
		nb = nb / 10;
	}
	return (str);
}

int	ft_len(long nb)
{
	int	len;

	len = 0;
	if (nb <= 0)
	{
		nb = -nb;
		len++;
	}
	while (nb > 0)
	{
		nb = nb / 10;
		len++;
	}
	return (len);
}

int	ft_fprintf(FILE *stream, const char *format, ...)
{
	va_list	args;
	int		fd;
	int		count;

	if (!format || !stream)
		return (-1);
	fd = ft_fileno(stream);
	if (fd < 0)
		return (-1);
	va_start(args, format);
	count = 0;
	while (*format)
	{
		if (*format == '%' && *(format + 1))
		{
			format++;
			handle_conversion(*format, args, fd);
		}
		else
			ft_putchar_fd(*format, fd);
		format++;
	}
	va_end(args);
	return (count);
}
