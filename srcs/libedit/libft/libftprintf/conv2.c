/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/11 17:12:10 by gdufay            #+#    #+#             */
/*   Updated: 2017/12/18 12:38:27 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_hhu(va_list va, char **format, t_option **option)
{
	unsigned char arg;

	*format += 2;
	arg = (unsigned char)va_arg(va, int);
	return (ft_itoa_base_u(arg, (*option)->base, (*option)->up));
}

char	*ft_hu(va_list va, char **format, t_option **option)
{
	unsigned short arg;

	*format += 1;
	arg = (unsigned short)va_arg(va, int);
	return (ft_itoa_base_u(arg, (*option)->base, (*option)->up));
}

char	*ft_lu(va_list va, char **format, t_option **option)
{
	unsigned long	arg;

	while (**format != 'u' && **format != 'U' && **format != 'O' &&
			**format != 'x' && **format != 'o' && **format != 'X')
		*format += 1;
	arg = va_arg(va, unsigned long);
	return (ft_itoa_base_u(arg, (*option)->base, (*option)->up));
}

char	*ft_llu(va_list va, char **format, t_option **option)
{
	unsigned long long arg;

	*format += 2;
	arg = va_arg(va, unsigned long long);
	return (ft_itoa_base_u(arg, (*option)->base, (*option)->up));
}

char	*ft_ju(va_list va, char **format, t_option **option)
{
	uintmax_t arg;

	*format += 1;
	arg = va_arg(va, uintmax_t);
	return (ft_itoa_base_u(arg, (*option)->base, (*option)->up));
}
