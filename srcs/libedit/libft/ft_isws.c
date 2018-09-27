/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isws.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/09 10:28:13 by gdufay            #+#    #+#             */
/*   Updated: 2018/07/09 10:32:02 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

inline int	ft_isws(int c)
{
	return (c == ' ' || c == '\t' || c == '\v'
			|| c == '\n' || c == '\f' || c == '\r');
}