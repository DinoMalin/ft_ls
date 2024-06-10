/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcario <jcario@student.42lehavre.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 16:13:06 by jcario            #+#    #+#             */
/*   Updated: 2023/12/09 19:36:52 by jcario           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	handle_ptr(int fd, void *ptr)
{
	if (!ptr)
		return (ft_putstr_fd("(nil)", fd));
	return (ft_puthexa_fd((unsigned long)ptr, fd, FALSE));
}

static int	ft_put_var(int fd, char type, va_list args)
{
	if (type == 'c')
		return (ft_putchar_fd((char)va_arg(args, int), fd));
	if (type == 's')
		return (ft_putstr_fd((char *)va_arg(args, char *), fd));
	if (type == 'p')
		return (handle_ptr(fd, (void *)va_arg(args, void *)));
	if (type == 'd')
		return (ft_putnbr_fd((int)va_arg(args, int), fd));
	if (type == 'i')
		return (ft_putnbr_fd((int)va_arg(args, int), fd));
	if (type == 'u')
		return (ft_putunbr_fd((unsigned int)va_arg(args, unsigned int), fd));
	if (type == 'x')
		return (ft_puthexa_fd((int)va_arg(args, int), fd, FALSE));
	if (type == 'X')
		return (ft_puthexa_fd((int)va_arg(args, int), fd, TRUE));
	if (type == '%')
		return (ft_putchar_fd('%', fd));
	return (0);
}

int	ft_fprintf(int fd, const char *str, ...)
{
	int		i;
	va_list	args;
	int		count;

	i = 0;
	count = 0;
	va_start(args, str);
	while (str[i])
	{
		if (str[i] == '%' && str[i + 1])
		{
			i++;
			count += ft_put_var(fd, str[i], args);
			i++;
		}
		if (str[i] && str[i] != '%')
		{
			ft_putchar_fd(str[i], fd);
			count++;
			i++;
		}
	}
	va_end(args);
	return (count);
}

// int main()
// {
// 	ft_printf("%d", ft_printf(" %x ", 16));
// }
