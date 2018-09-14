/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fmax.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/07 18:00:20 by mmerabet          #+#    #+#             */
/*   Updated: 2018/02/08 18:19:13 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_math.h"

float	ft_fmax(float a, float b)
{
	return (a > b ? a : b);
}

float	ft_fmin(float a, float b)
{
	return (a < b ? a : b);
}
