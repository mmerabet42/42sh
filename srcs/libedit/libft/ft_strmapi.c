/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 17:26:43 by gdufay            #+#    #+#             */
/*   Updated: 2017/11/09 17:30:38 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*d;
	char			*map;

	if (!s || !f)
		return (NULL);
	if (!(map = (char*)malloc(sizeof(*map) * (ft_strlen(s) + 1))))
		return (NULL);
	d = map;
	i = 0;
	while (*s)
		*map++ = f(i++, *s++);
	*map = 0;
	return (d);
}
