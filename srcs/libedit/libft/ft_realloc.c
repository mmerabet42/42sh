/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/15 10:42:20 by gdufay            #+#    #+#             */
/*   Updated: 2017/11/15 12:15:42 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void *ptr, size_t size)
{
	void *tab;

	if (!ptr)
		return (malloc(size));
	if (!size && ptr)
	{
		free(ptr);
		return (ptr);
	}
	tab = malloc(sizeof(*ptr) * size);
	return (ft_memmove(tab, ptr, size));
}
