/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/07 15:21:43 by gdufay            #+#    #+#             */
/*   Updated: 2017/12/15 11:00:59 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_atoi(const char *str)
{
	int					sign;
	unsigned long long	res;

	res = 0;
	sign = 1;
	while (*str == '\n' || *str == '\t' || *str == '\v' || *str == '\f'
		|| *str == '\r' || *str == ' ')
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str && (*str >= '0' && *str <= '9'))
	{
		res = res * 10 + (*str - '0');
		str++;
	}
	if (res > 9223372036854775807)
		return (sign == 1 ? -1 : 0);
	return ((int)(res * sign));
}
