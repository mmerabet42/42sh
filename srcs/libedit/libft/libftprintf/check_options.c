/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_options.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/06 11:26:59 by gdufay            #+#    #+#             */
/*   Updated: 2017/12/18 14:37:53 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_check_flag(t_option **option, char **format)
{
	while (**format && (**format == '#' || **format == '0' || **format == '-'
				|| **format == '+' || **format == ' '))
	{
		if (!ft_strchr((*option)->flag, **format))
			(*option)->flag[ft_strlen((*option)->flag)] = **format;
		*format += 1;
	}
	ft_check_field(option, format);
}

void	ft_check_field(t_option **option, char **format)
{
	if (ft_atoi(*format))
	{
		(*option)->field = (int)ft_atoi(*format);
		*format += ft_intlen((*option)->field);
	}
	ft_check_precision(option, format);
}

void	ft_check_precision(t_option **option, char **format)
{
	int i;

	if (**format == '.')
	{
		*format += 1;
		i = ft_atoi(*format);
		if (i >= 0)
			(*option)->prec = i;
		else
			(*option)->prec = 0;
		while (ft_isdigit(**format) || **format == '-')
			*format += 1;
	}
	ft_clean_option(option);
}

void	ft_clean_option(t_option **option)
{
	int i;

	i = -1;
	if (ft_strchr((*option)->flag, '0') && ft_strchr((*option)->flag, '-'))
		while ((*option)->flag[++i])
			if ((*option)->flag[i] == '0')
				(*option)->flag[i] = 'a';
	i = -1;
	if (ft_strchr((*option)->flag, '+') && ft_strchr((*option)->flag, ' '))
		while ((*option)->flag[++i])
			if ((*option)->flag[i] == ' ')
				(*option)->flag[i] = 'a';
	i = -1;
	if (ft_strchr((*option)->flag, '0') && (*option)->prec >= 0)
		while ((*option)->flag[++i])
			if ((*option)->flag[i] == '0')
				(*option)->flag[i] = 'a';
	i = -1;
	if (ft_strchr((*option)->flag, '#') && (ft_strchr((*option)->flag, ' ') ||
				ft_strchr((*option)->flag, '+')))
		while ((*option)->flag[++i])
			if ((*option)->flag[i] == '+' || (*option)->flag[i] == ' ')
				(*option)->flag[i] = 'a';
}
