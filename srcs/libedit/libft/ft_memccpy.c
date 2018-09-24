/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/08 19:03:37 by gdufay            #+#    #+#             */
/*   Updated: 2017/11/14 16:04:39 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dest, const void *src, int c, size_t n)
{
	unsigned char	*m;
	unsigned char	*d;
	size_t			i;

	d = (unsigned char*)dest;
	m = (unsigned char*)src;
	i = -1;
	while (++i < n)
	{
		d[i] = m[i];
		if (m[i] == (unsigned char)c)
			return (dest + i + 1);
	}
	return (NULL);
}
