/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_elemacc_simplified.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/25 16:36:10 by sle-rest          #+#    #+#             */
/*   Updated: 2018/09/15 18:06:33 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"
#include "ft_str.h"

char	*get_elemacc_simplified(char *str, int *x, int y)
{
	char	*res;
	int		len;
	int		j;

	len = 0;
	while (detect_globbing_at(str, len) != accolade)
		len++;
	len += len_between_comma(str, *x);
	if (!(res = (char *)malloc(sizeof(char) * (len + 1))))
		return (0);
	j = -1;
	while (detect_globbing_at(str, ++j) != accolade)
		res[j] = str[j];
	len = len_between_comma(str, *x);
	while (len--)
	{
		res[j] = str[*x];
		j++;
		(*x)++;
	}
	*x = str[*x] == ',' ? *x + 1 : *x;
	res[j] = '\0';
	ft_strcat(res, str + y);
	return (res);
}
