/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 17:21:42 by gdufay            #+#    #+#             */
/*   Updated: 2017/11/09 17:41:43 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmap(char const *s, char (*f)(char))
{
	char *map;
	char *d;

	if (!s || !f)
		return (NULL);
	if (!(map = (char*)malloc(sizeof(*map) * (ft_strlen(s) + 1))))
		return (NULL);
	d = map;
	while (*s)
		*map++ = f(*s++);
	*map = 0;
	return (d);
}
