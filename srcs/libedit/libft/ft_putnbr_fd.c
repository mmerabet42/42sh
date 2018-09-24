/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 11:16:02 by gdufay            #+#    #+#             */
/*   Updated: 2017/11/12 15:59:23 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int nb;

	if (fd)
	{
		nb = n;
		if (n < 0)
		{
			nb = -nb;
			ft_putchar_fd('-', fd);
		}
		if (nb < 10)
			ft_putchar_fd(nb + 48, fd);
		else
		{
			ft_putnbr_fd(nb / 10, fd);
			ft_putnbr_fd(nb % 10, fd);
		}
	}
}
