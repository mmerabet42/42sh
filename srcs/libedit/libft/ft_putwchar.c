/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putwchar.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/07 14:29:59 by gdufay            #+#    #+#             */
/*   Updated: 2017/12/15 16:41:42 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putwchar(wchar_t c)
{
	if (c <= 0x007f)
		ft_putchar(c);
	else if (c <= 0x07ff && MB_CUR_MAX > 1)
	{
		ft_putchar((c >> 6) + 0xc0);
		ft_putchar((c & 0x3f) + 0x80);
	}
	else if (c <= 0xffff && MB_CUR_MAX > 1)
	{
		ft_putchar((c >> 12) + 0xe0);
		ft_putchar((c >> 6 & 0x3f) + 0x80);
		ft_putchar((c & 0x3f) + 0x80);
	}
	else if (c <= 0x10ffff && MB_CUR_MAX > 1)
	{
		ft_putchar((c >> 18) + 0xf0);
		ft_putchar((c >> 12 & 0x3f) + 0x80);
		ft_putchar((c >> 6 & 0x3f) + 0x80);
		ft_putchar((c & 0x3f) + 0x80);
	}
	else
		ft_putchar((char)c);
}
