/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 17:00:31 by gdufay            #+#    #+#             */
/*   Updated: 2017/11/11 12:10:24 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnew(size_t size)
{
	char *new;
	char *n;

	if (!(new = (char*)malloc(sizeof(*new) * size + 1)))
		return (NULL);
	n = new;
	while (size--)
		*new++ = 0;
	*new = 0;
	return (n);
}
