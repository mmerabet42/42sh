/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 14:36:20 by gdufay            #+#    #+#             */
/*   Updated: 2018/01/19 12:18:28 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s)
{
	char	*dup;
	int		len;
	int		i;

	if (!s)
		return (NULL);
	while ((*s == ' ' || *s == '\n' || *s == '\t') && *s)
		s++;
	i = ft_strlen(s) - 1;
	while ((s[i] == ' ' || s[i] == '\n' ||
				s[i] == '\t') && s[i] != *s && i != -1)
		i--;
	len = i;
	i = -1;
	if (!(dup = (char*)malloc(sizeof(*dup) * (len + 2))))
		return (NULL);
	while (++i <= len && s[i])
		dup[i] = s[i];
	dup[i] = '\0';
	return (dup);
}
