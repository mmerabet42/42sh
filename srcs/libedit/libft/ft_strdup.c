/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/08 16:08:00 by gdufay            #+#    #+#             */
/*   Updated: 2018/01/26 11:26:38 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*dup;
	int		i;

	if (!s1 || !*s1)
		return (NULL);
	if (!(dup = (char*)malloc(sizeof(*dup) * (ft_strlen(s1) + 1))))
		return (NULL);
	i = -1;
	while (*s1)
		dup[++i] = *s1++;
	dup[++i] = 0;
	return (dup);
}
