/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/04 15:38:10 by gdufay            #+#    #+#             */
/*   Updated: 2018/01/17 11:09:39 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void		ft_init(t_option **opt)
{
	t_option *option;

	if (!(option = malloc(sizeof(*option))))
		return ;
	option->flag = ft_strnew(5);
	option->field = 0;
	option->prec = -1;
	option->arg = "";
	option->carac = '\0';
	option->base = 10;
	option->up = 0;
	option->is_neg = 0;
	*opt = option;
}

void		ft_clear(t_option **option)
{
	if ((*option)->arg)
		ft_strdel(&(*option)->arg);
	ft_strdel(&(*option)->flag);
	free(*option);
	*option = NULL;
}

int			fti(va_list va, char **format)
{
	int				tab[3];
	t_option		*option;
	static t_conv	liste_conv[] = {
		{'d', ft_signed}, {'i', ft_signed}, {'o', ft_unsigned},
		{'u', ft_unsigned}, {'x', ft_unsigned}, {'X', ft_unsigned},
		{'s', ft_s}, {'c', ft_c}, {'%', ft_mod}, {'p', ft_p}, {'D', ft_signed},
		{'U', ft_unsigned}, {'O', ft_unsigned}, {'C', ft_c}, {'S', ft_s}
	};

	tab[0] = -1;
	tab[1] = -2;
	*format += 1;
	ft_init(&option);
	ft_check_flag(&option, format);
	tab[2] = 1;
	while (++tab[0] <= 15)
	{
		tab[0] = (tab[0] == 15 && ++tab[2] < 4 ? 0 : tab[0]);
		if (tab[2] == 4 || tab[1] > -2)
			break ;
		if (tab[0] >= 0 && ft_strnchr(*format, liste_conv[tab[0]].elem, tab[2]))
			tab[1] = liste_conv[tab[0]].f(va, format, option);
	}
	ft_clear(&option);
	return (tab[1]);
}

int			ft_printf(const char *format, ...)
{
	va_list		va;
	int			i;
	int			j;
	char		*form;

	i = 0;
	j = 0;
	form = (char*)format;
	va_start(va, format);
	while (*form && (++i))
	{
		if (*form == '%')
		{
			i--;
			if ((j = fti(va, &form)) >= 0)
				i += j;
			else
				return (-1);
		}
		else
			ft_putchar_fd(*form++, 1);
	}
	va_end(va);
	return (i);
}
