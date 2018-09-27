/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/08 13:25:40 by gdufay            #+#    #+#             */
/*   Updated: 2017/11/17 12:19:55 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncat(char *s1, const char *s2, size_t n)
{
	unsigned int i;
	unsigned int len;

	len = 0;
	while (s1[len])
		len++;
	i = -1;
	if (s2)
		while (s2[++i] && i < n)
			s1[i + len] = s2[i];
	s1[i + len] = '\0';
	return (s1);
}
