/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/08 17:31:37 by gdufay            #+#    #+#             */
/*   Updated: 2017/11/12 14:38:23 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*u;

	u = (unsigned char*)s;
	while (n--)
	{
		if (*u == (unsigned char)c)
			return (u);
		u++;
	}
	return (NULL);
}
