/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conv1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/11 16:53:42 by gdufay            #+#    #+#             */
/*   Updated: 2017/12/18 13:14:27 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_hh(va_list va, char **format, t_option **option)
{
	char			arg;

	while (**format != 'i' && **format != 'd' && **format != 'D')
		*format += 1;
	arg = (char)va_arg(va, int);
	if (arg < 0)
		(*option)->is_neg = 1;
	return (ft_itoa_base_u((arg < 0 ? -arg : arg), 10, 0));
}

char	*ft_h(va_list va, char **format, t_option **option)
{
	short			arg;

	while (**format != 'i' && **format != 'd' && **format != 'D')
		*format += 1;
	arg = (short)va_arg(va, int);
	if (arg < 0)
		(*option)->is_neg = 1;
	return (ft_itoa_base_u((arg < 0 ? -arg : arg), 10, 0));
}

char	*ft_l(va_list va, char **format, t_option **option)
{
	long	arg;

	while (**format != 'D' && **format != 'd' && **format != 'i')
		*format += 1;
	arg = va_arg(va, long);
	if (arg < 0)
		(*option)->is_neg = 1;
	return (ft_itoa_base_u((arg < 0 ? -arg : arg), 10, 0));
}

char	*ft_ll(va_list va, char **format, t_option **option)
{
	long long	arg;

	while (**format != 'i' && **format != 'd' && **format != 'D')
		*format += 1;
	arg = va_arg(va, long long);
	if (arg < 0)
		(*option)->is_neg = 1;
	return (ft_itoa_base_u((arg < 0 ? -arg : arg), 10, 0));
}

char	*ft_j(va_list va, char **format, t_option **option)
{
	intmax_t	arg;

	while (**format != 'i' && **format != 'd' && **format != 'D')
		*format += 1;
	arg = va_arg(va, intmax_t);
	if (arg < 0)
		(*option)->is_neg = 1;
	return (ft_itoa_base_u((arg < 0 ? -arg : arg), 10, 0));
}
