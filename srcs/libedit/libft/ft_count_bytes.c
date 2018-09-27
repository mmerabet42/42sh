/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_bytes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/07 15:01:45 by gdufay            #+#    #+#             */
/*   Updated: 2017/12/14 17:13:00 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_count_bytes(wchar_t n)
{
	if (n <= 0x00ff)
		return (1);
	else if (n <= 0x07ff)
		return (2);
	else if (n <= 0xffff)
		return (3);
	else
		return (4);
}
