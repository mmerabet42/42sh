/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 15:43:57 by gdufay            #+#    #+#             */
/*   Updated: 2017/11/14 15:39:08 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	int		i;

	if (!s)
		return (NULL);
	if (!(sub = (char*)malloc(sizeof(*sub) * (len + 1))))
		return (NULL);
	i = 0;
	while (len--)
		sub[i++] = s[start++];
	sub[i] = '\0';
	return (sub);
}
