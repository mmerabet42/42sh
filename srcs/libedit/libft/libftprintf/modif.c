/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modif.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/06 17:03:08 by gdufay            #+#    #+#             */
/*   Updated: 2018/01/17 11:09:41 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_malloc(t_option *option, int len)
{
	int		i;

	i = ((int)option->field < option->prec ? option->prec : option->field);
	i = (len < i ? i : len);
	if (ft_strchr(option->flag, '#') || option->carac == 'p')
		return (ft_strnew(i + 2));
	if (ft_strchr(option->flag, ' ')
			|| ft_strchr(option->flag, '+') || option->is_neg)
		return (ft_strnew(i + 1));
	return (ft_strnew(i));
}

int		ft_intfill(t_option *option)
{
	char	flag;
	char	*tab;
	int		i;

	tab = ft_malloc(option, ft_strlen(option->arg));
	flag = (ft_strchr(option->flag, '#') ? '#' : 0);
	flag = (option->is_neg && !flag ? '-' : flag);
	flag = (!flag && ft_strchr(option->flag, ' ') ? ' ' : flag);
	flag = (!flag && ft_strchr(option->flag, '+') ? '+' : flag);
	if (ft_strchr(option->flag, '-'))
		ft_fill_int_right(&tab, option, flag);
	else if (ft_strchr(option->flag, '0'))
		ft_fill_int_left(&tab, option, 1, flag);
	else
		ft_fill_int_left(&tab, option, 0, flag);
	if (tab)
		ft_putstr_fd(tab, 1);
	i = ft_strlen(tab);
	ft_strdel(&tab);
	return (i);
}

int		ft_charfill(t_option *option)
{
	char	*tab;
	int		i;

	tab = ft_malloc(option, ft_strlen(option->arg));
	if (ft_strchr(option->flag, '-'))
		ft_fill_char_right(&tab, option);
	else if (ft_strchr(option->flag, '0'))
		ft_fill_char_left(&tab, option, 1);
	else
		ft_fill_char_left(&tab, option, 0);
	ft_putstr_fd(tab, 1);
	if (!*option->arg && !ft_strchr(option->flag, '-') && option->carac == 'c')
		ft_putchar_fd(*option->arg, 1);
	i = ft_strlen(tab);
	ft_strdel(&tab);
	return (i + (!*option->arg && option->carac == 'c' ? 1 : 0));
}
