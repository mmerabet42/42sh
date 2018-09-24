/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/14 10:17:46 by gdufay            #+#    #+#             */
/*   Updated: 2017/12/18 15:06:14 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_z(va_list va, char **format, t_option **option)
{
	ssize_t arg;

	*format += 1;
	arg = va_arg(va, ssize_t);
	if (arg < 0)
		(*option)->is_neg = 1;
	return (ft_itoa_base_u((arg < 0 ? -arg : arg), 10, 0));
}

char	*ft_d(va_list va, char **format, t_option **option)
{
	int				arg;

	(void)format;
	arg = va_arg(va, int);
	if (arg < 0)
		(*option)->is_neg = 1;
	return (ft_itoa_base((arg < 0 ? -arg : arg), 10, 0));
}

char	*ft_zu(va_list va, char **format, t_option **option)
{
	size_t arg;

	*format += 1;
	arg = va_arg(va, size_t);
	return (ft_itoa_base_u(arg, (*option)->base, (*option)->up));
}

char	*ft_u(va_list va, char **format, t_option **option)
{
	unsigned int arg;

	(void)format;
	arg = va_arg(va, unsigned int);
	return (ft_itoa_base_u(arg, (*option)->base, (*option)->up));
}
