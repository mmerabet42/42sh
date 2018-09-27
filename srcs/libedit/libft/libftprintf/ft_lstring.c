/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/14 11:03:59 by gdufay            #+#    #+#             */
/*   Updated: 2017/12/18 14:05:45 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static	void	gain_place1(t_option **option, int *j, wchar_t *arg)
{
	int k;

	k = 0;
	while (arg[k] && (*option)->prec >= ft_count_bytes(arg[k]))
	{
		(*option)->prec -= ft_count_bytes(arg[k]);
		ft_putwchar(arg[k++]);
	}
	*j -= (*option)->prec;
	k = 0;
	while (k++ < (*option)->field - *j)
		ft_putchar(' ');
	*j += (k - 1);
}

static	void	gain_place2(t_option **option, int *j, wchar_t *arg)
{
	int k;

	k = 0;
	if (arg[0])
		while ((*option)->prec && ++k < (*option)->field
				- (*option)->prec + ft_count_bytes(arg[0]))
			ft_putchar((ft_strchr((*option)->flag, '0') ? '0' : ' '));
	*j = (*option)->prec + k - (arg[0] && (*option)->prec ? 1 : 0);
	k = 0;
	while (arg[k] && (*option)->prec >= ft_count_bytes(arg[k]))
	{
		(*option)->prec -= ft_count_bytes(arg[k]);
		ft_putwchar(arg[k++]);
	}
	*j -= (*option)->prec;
}

int				ft_check(wchar_t *arg, int prec)
{
	int k;

	k = 0;
	while (arg[k] && prec >= ft_count_bytes(arg[k]))
	{
		if (arg[k] <= 0x10ffff && arg[k] >= 0
				&& (arg[k] <= 0xd7ff || arg[k] >= 0xe000)
				&& ft_count_bytes(arg[k]) <= MB_CUR_MAX)
		{
			prec -= ft_count_bytes(arg[k]);
			k++;
		}
		else
			return (0);
	}
	return (1);
}

int				ft_lstring(va_list va, char **format, t_option *option)
{
	wchar_t *arg;
	int		j;
	int		len_arg;

	while (**format != 's' && **format != 'S')
		*format += 1;
	*format += 1;
	if (!(arg = va_arg(va, wchar_t*)))
	{
		ft_putstr("(null)");
		return (6);
	}
	len_arg = ft_strwlen(arg);
	if (option->prec == -1 || option->prec > len_arg)
		option->prec = len_arg;
	j = option->prec;
	if (!ft_check(arg, option->prec))
		return (-1);
	if (ft_strchr(option->flag, '-'))
		gain_place1(&option, &j, arg);
	else
		gain_place2(&option, &j, arg);
	return (j);
}
