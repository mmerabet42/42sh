/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base_u.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/11 17:17:50 by gdufay            #+#    #+#             */
/*   Updated: 2017/12/12 13:13:10 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa_base_u(uintmax_t n, int base, int up)
{
	char		*tab;
	int			i;
	uintmax_t	nb;

	i = 1;
	nb = n;
	while ((nb /= base))
		i++;
	if (!(tab = (char*)malloc(sizeof(*tab) * (i + 1))))
		return (NULL);
	tab[i] = 0;
	while (i--)
	{
		nb = (up ? 'A' : 'a');
		tab[i] = (n % base < 10 ? n % base + 48 : n % base + nb - 10);
		n /= base;
	}
	return (tab);
}
