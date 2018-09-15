/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_no_backslash.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/06 23:00:05 by sle-rest          #+#    #+#             */
/*   Updated: 2018/09/14 16:48:06 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_no_backslash(char *str, int i)
{
	if (!i || str[i - 1] != '\\')
		return (1);
	return (0);
}
