/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/07 16:57:44 by gdufay            #+#    #+#             */
/*   Updated: 2017/11/11 11:44:44 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	char	*ptr;
	int		i;

	if (!(*needle))
		return ((char*)haystack);
	while (*haystack)
	{
		i = 0;
		ptr = (char*)haystack;
		while (*haystack == needle[i] && *haystack && needle[i])
		{
			haystack++;
			i++;
			if (!(needle[i]))
				return (ptr);
		}
		haystack = ptr++;
		haystack++;
	}
	return (NULL);
}
