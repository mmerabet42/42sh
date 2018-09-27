/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_int.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/05 12:09:28 by gdufay            #+#    #+#             */
/*   Updated: 2017/12/18 15:33:10 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		ft_signed(va_list va, char **format, t_option *option)
{
	int				i;
	static t_long	liste_long[] = {
		{"hhD", ft_ll}, {"lD", ft_ll}, {"D", ft_l},
		{"hh", ft_hh}, {"h", ft_h}, {"ll", ft_ll}, {"l", ft_l},
		{"j", ft_j}, {"z", ft_z}, {"d", ft_d}
	};

	i = -1;
	while (++i < 10)
		if (i == 9 || ft_strnstr(*format, liste_long[i].elem, 3))
		{
			option->arg = liste_long[i].f(va, format, &option);
			break ;
		}
	option->carac = **format;
	*format += 1;
	return (ft_intfill(option));
}

char	ft_select(char *format, int *base, int *up)
{
	*up = 0;
	if (ft_strnchr(format, 'x', 3))
	{
		*base = 16;
		return ('x');
	}
	else if (ft_strnchr(format, 'X', 3))
	{
		*base = 16;
		*up = 1;
		return ('X');
	}
	else if (ft_strnchr(format, 'o', 3) || ft_strnchr(format, 'O', 3))
	{
		*base = 8;
		return ('o');
	}
	else
	{
		*base = 10;
		return ('u');
	}
}

int		ft_unsigned(va_list va, char **format, t_option *option)
{
	int				i;
	static t_long	liste_long_u[] = {
		{"hhU", ft_lu}, {"hhO", ft_lu}, {"hO", ft_lu}, {"O", ft_lu},
		{"U", ft_lu}, {"hh", ft_hhu}, {"h", ft_hu}, {"ll", ft_llu},
		{"l", ft_lu}, {"j", ft_ju}, {"z", ft_zu}, {"uoxX", ft_u}
	};

	i = -1;
	option->carac = ft_select(*format, &option->base, &option->up);
	while (++i < 12)
		if (i == 11 || ft_strnstr(*format, liste_long_u[i].elem, 3))
		{
			option->arg = liste_long_u[i].f(va, format, &option);
			break ;
		}
	*format += 1;
	return (ft_intfill(option));
}
