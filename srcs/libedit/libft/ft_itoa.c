/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 11:40:11 by gdufay            #+#    #+#             */
/*   Updated: 2017/11/12 15:43:29 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa(int n)
{
	char			*tab;
	int				i;
	unsigned int	nb;
	unsigned int	a;
	int				cond;

	i = (n < 0 ? 2 : 1);
	cond = (n < 0 ? 1 : 0);
	a = (n < 0 ? -n : n);
	nb = a;
	while ((nb /= 10))
		i++;
	if (!(tab = (char*)malloc(sizeof(*tab) * (i + 1))))
		return (NULL);
	tab[i] = 0;
	while (i-- > cond)
	{
		tab[i] = a % 10 + 48;
		a /= 10;
	}
	if (i >= 0)
		tab[0] = '-';
	return (tab);
}
