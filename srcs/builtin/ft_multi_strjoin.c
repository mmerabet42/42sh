/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_multi_strjoin.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/08 16:33:59 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/08 17:32:48 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_str.h"

static size_t	fill_address_ap(char ***tmp, int n, va_list ap)
{
	int		i;
	size_t	len;

	i = -1;
	len = 0;
	if (!(*tmp = (char **)malloc(sizeof(char *) * n)))
		return (0);
	while (++i < n)
	{
		(*tmp)[i] = va_arg(ap, char *);
		len += ft_strlen((*tmp)[i]);
	}
	return (len);
}

char			*ft_multi_strjoin(int n, ...)
{
	va_list	ap;
	int		i;
	char	**tmp;
	char	*res;
	size_t	len;

	if (n <= 0)
		return (NULL);
	va_start(ap, n);
	if (!(len = fill_address_ap(&tmp, n, ap)))
		return (NULL);
	va_end(ap);
	if (!(res = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	*res = '\0';
	i = -1;
	while (++i < n)
		ft_strcat(res, tmp[i]);
	free(tmp);
	return (res);
}

void			ft_strremove(char *s)
{
	char	*d;
	char	c;

	d = s;
	while (s && *s)
	{
		c = *s++;
		if (c != '\\' || (c == '\\' && *s == '\\'))
		{
			*d++ = c;
			if (c == '\\' && *s == '\\')
				s++;
		}
		if (!c)
			break ;
	}
	if (d)
		*d = 0;
}
