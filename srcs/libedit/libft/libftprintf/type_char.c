/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_char.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/05 14:33:38 by gdufay            #+#    #+#             */
/*   Updated: 2017/12/19 13:09:39 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		ft_s(va_list va, char **format, t_option *option)
{
	const char *s;

	if (**format == 'l' || ft_strnchr(*format, 'S', 2))
		return (ft_lstring(va, format, option));
	*format += 1;
	s = va_arg(va, const char*);
	if (!(option->arg = ft_strdup((s ? s : "(null)"))))
		return (0);
	if (option->prec == -1)
		option->prec = ft_strlen(option->arg);
	option->carac = 's';
	return (ft_charfill(option));
}

int		ft_c(va_list va, char **format, t_option *option)
{
	unsigned char c;

	if (**format == 'l' || ft_strnchr(*format, 'C', 2))
		return (ft_lc(va, format, option));
	*format += 1;
	c = (unsigned char)va_arg(va, int);
	option->arg = ft_strnew(1);
	*option->arg = c;
	option->prec = 1;
	option->carac = 'c';
	if (!c && ft_strchr(option->flag, '-'))
		ft_putchar(c);
	return (ft_charfill(option));
}

int		ft_mod(va_list va, char **format, t_option *option)
{
	*format += 1;
	(void)va;
	if (!(option->arg = ft_strdup("%")))
		return (0);
	option->carac = '%';
	if (option->prec == -1)
		option->prec = 1;
	return (ft_charfill(option));
}

int		ft_p(va_list va, char **format, t_option *option)
{
	long long	arg;

	arg = (long long)va_arg(va, void*);
	while (**format != 'p')
		*format += 1;
	*format += 1;
	if (!(option->arg = ft_itoa_base(arg, 16, 0)))
		return (0);
	option->carac = 'p';
	return (ft_charfill(option));
}

int		ft_lc(va_list va, char **format, t_option *option)
{
	wchar_t arg;
	int		i;

	while (**format != 'C' && **format != 'c')
		*format += 1;
	*format += 1;
	i = -1;
	arg = (wchar_t)va_arg(va, wint_t);
	if (!(arg <= 0x10ffff && arg >= 0 && (arg <= 0xd7ff
				|| arg >= 0xe000) && ft_count_bytes(arg) <= MB_CUR_MAX))
		return (-1);
	if ((i = -1) && ft_strchr(option->flag, '-'))
	{
		ft_putwchar(arg);
		while (++i < option->field - ft_count_bytes(arg))
			ft_putchar(' ');
	}
	else
	{
		while (++i < option->field - ft_count_bytes(arg))
			ft_putchar((ft_strchr(option->flag, '0') ? '0' : ' '));
		ft_putwchar(arg);
	}
	return (i + ft_count_bytes(arg));
}
