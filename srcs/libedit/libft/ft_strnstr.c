/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/07 17:45:56 by gdufay            #+#    #+#             */
/*   Updated: 2017/11/14 15:27:21 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	char	*ptr;
	int		i;
	int		j;

	if (!(*needle))
		return ((char*)haystack);
	while (*haystack && len)
	{
		i = 0;
		ptr = (char*)haystack;
		j = len;
		while (*haystack == needle[i] && *haystack && needle[i] && len)
		{
			haystack++;
			i++;
			len--;
			if (!(needle[i]))
				return (ptr);
		}
		haystack = (i != 0 ? ptr + 1 : haystack + 1);
		len = (i != 0 ? j - 1 : len - 1);
	}
	return (NULL);
}
