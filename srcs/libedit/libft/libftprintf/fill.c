/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/06 18:05:32 by gdufay            #+#    #+#             */
/*   Updated: 2017/12/18 14:22:02 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void			ft_fill_int_right(char **tab, t_option *option, char flag)
{
	int len;
	int	i;
	int	j;

	len = ft_strlen(option->arg);
	option->prec = (option->prec < len ? len : option->prec);
	if (flag == '#')
	{
		if (option->carac == 'x' && ft_atoi(option->arg))
			*tab = ft_strcpy(*tab, "0x");
		if (option->carac == 'X' && ft_atoi(option->arg))
			*tab = ft_strcpy(*tab, "0X");
		if (option->carac == 'o' && ft_atoi(option->arg))
			*tab = ft_strcpy(*tab, "0");
	}
	i = ft_strlen(*tab);
	if (flag && flag != '#' && ft_atoi(option->arg) >= 0)
		tab[0][i++] = flag;
	j = i;
	while (i < option->prec - len + j)
		tab[0][i++] = '0';
	*tab = (option->prec ? ft_strncat(*tab, option->arg, len) : *tab);
	i = ft_strlen(*tab);
	while (i < option->field)
		tab[0][i++] = ' ';
}

static	int		gain_de_place(t_option *option, char **tab, int zero)
{
	int i;

	i = 0;
	if (*option->arg != '0' && !zero)
		option->field -= (option->carac == 'o' ? 1 : 2);
	if (!zero)
		while (i < option->field - option->prec)
			tab[0][i++] = ' ';
	if (option->carac == 'x' && *option->arg != '0')
		*tab = ft_strcat(*tab, "0x");
	if (option->carac == 'X' && *option->arg != '0')
		*tab = ft_strcat(*tab, "0X");
	if (option->carac == 'o' && *option->arg != '0')
		*tab = ft_strcat(*tab, "0");
	i = ft_strlen(*tab);
	if (zero)
		while (i < option->field - option->prec)
			tab[0][i++] = '0';
	return (i - (option->carac == 'o' && *option->arg != '0' ? 1 : 0));
}

void			ft_fill_int_left(char **tab, t_option *opt,
		int zero, char flag)
{
	int	len_arg;
	int	i;
	int	j;

	len_arg = ft_strlen(opt->arg);
	if ((opt->prec || *opt->arg != '0') && opt->prec < len_arg)
		opt->prec = len_arg;
	if (!(i = 0) && flag == '#')
		i = gain_de_place(opt, tab, zero);
	else
	{
		if (flag && flag != '#' && ft_atoi(opt->arg) >= 0 && zero)
			tab[0][i++] = flag;
		while (i < opt->field - opt->prec - (flag && !zero ? 1 : 0))
			tab[0][i++] = (zero ? '0' : ' ');
	}
	if (flag && flag != '#' && (ft_atoi(opt->arg) >= 0
				|| (opt->is_neg && *opt->arg != '-')) && !zero)
		tab[0][i++] = flag;
	j = i;
	while (i < opt->prec - len_arg + j)
		tab[0][i++] = '0';
	if (opt->prec || (flag == '#' && opt->carac == 'o')
			|| ft_atoi(opt->arg) || *opt->arg != '0')
		*tab = ft_strncat(*tab, opt->arg, len_arg);
}

void			ft_fill_char_right(char **tab, t_option *option)
{
	int i;
	int j;

	if (option->carac == 'p')
		*tab = ft_strcpy(*tab, "0x");
	*tab = ft_strncat(*tab, option->arg, option->prec);
	i = ft_strlen(*tab);
	j = i;
	while (i < option->field - (!*option->arg && option->carac == 'c' ? 1 : 0))
		tab[0][i++] = ' ';
}

void			ft_fill_char_left(char **tab, t_option *option, int zero)
{
	int i;

	i = -1;
	if (option->prec < (int)ft_strlen(option->arg) && option->carac != 's')
		option->prec = (int)ft_strlen(option->arg);
	while (++i < option->field - option->prec -
			(option->carac == 'p' ? 2 : 0))
		tab[0][i] = (zero ? '0' : ' ');
	if (option->carac == 'p')
		*tab = ft_strcat(*tab, "0x");
	if (!*option->arg && (option->carac == 's' || option->carac == '%'))
		while (i < option->field)
			tab[0][i++] = (zero ? '0' : ' ');
	*tab = ft_strncat(*tab, option->arg, option->prec);
}
