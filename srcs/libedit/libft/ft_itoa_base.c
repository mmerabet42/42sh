/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/05 13:23:17 by gdufay            #+#    #+#             */
/*   Updated: 2017/12/11 12:19:50 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa_base(intmax_t n, int base, int up)
{
	char		*tab;
	int			i;
	uintmax_t	nb;
	uintmax_t	a;
	int			cond;

	i = (n < 0 ? 2 : 1);
	cond = (n < 0 ? 1 : 0);
	a = (n < 0 ? -n : n);
	nb = a;
	while ((nb /= base))
		i++;
	if (!(tab = (char*)malloc(sizeof(*tab) * (i + 1))))
		return (NULL);
	tab[i] = 0;
	while (i-- > cond)
	{
		nb = (up ? 'A' : 'a');
		tab[i] = (a % base < 10 ? a % base + 48 : a % base + nb - 10);
		a /= base;
	}
	if (i >= 0)
		tab[0] = '-';
	return (tab);
}
