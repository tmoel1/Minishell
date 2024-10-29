/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility_functions_4.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astoll <astoll@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:26:45 by astoll            #+#    #+#             */
/*   Updated: 2024/10/22 13:26:47 by astoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_fileno(FILE *stream)
{
	if (stream == stdin)
		return (0);
	if (stream == stdout)
		return (1);
	if (stream == stderr)
		return (2);
	return (-1);
}

void	ft_putstr_fd(const char *str, int fd)
{
	write(fd, str, ft_strlen(str));
}

void	ft_putnbr_fd(int n, int fd)
{
	char	num_str[12];
	int		i;
	long	nb;

	nb = n;
	i = 0;
	if (nb == 0)
		num_str[i++] = '0';
	if (nb < 0)
	{
		ft_putchar_fd('-', fd);
		nb = -nb;
	}
	while (nb > 0)
	{
		num_str[i++] = (nb % 10) + '0';
		nb /= 10;
	}
	while (i--)
		ft_putchar_fd(num_str[i], fd);
}

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	handle_conversion(char c, va_list args, int fd)
{
	if (c == 'd')
		ft_putnbr_fd(va_arg(args, int), fd);
	else if (c == 's')
		ft_putstr_fd(va_arg(args, const char *), fd);
	else if (c == 'c')
		ft_putchar_fd((char)va_arg(args, int), fd);
	else if (c == '%')
		ft_putchar_fd('%', fd);
}
