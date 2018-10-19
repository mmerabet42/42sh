/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   regex_bracket.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 19:26:38 by mmerabet          #+#    #+#             */
/*   Updated: 2018/10/19 19:26:43 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_str.h"
#include "ft_printf.h"

static int	bracket_start(const char *str)
{
	if ((str[0] == '*' || str[0] == '?')
			&& (str[1] == '[' || (str[1] == '!' && str[2] == '[')))
		return (str[1] == '!' ? 3 : 2);
	return (0);
}

static int	bracket_end(const char *str, int l)
{
	if (l)
	{
		while (*str == '}' && l--)
			++str;
		if (!l && *str == ']')
			return (1);
	}
	else if (*str == ']')
		return (1);
	return (0);
}

int			regex_bracket(const char *str, int *s)
{
	int	i;
	int	j;
	int	l;

	if (!(i = bracket_start(str)))
		return (-2);
	l = 0;
	while (str[i] == '{' && ++l)
		++i;
	if (s)
		*s = i;
	while (str[i])
	{
		if (bracket_end(&str[i], l))
			return (i + l + 1);
		else if ((j = regex_bracket(&str[i], NULL)) == -1)
			return (-1);
		else if (j != -2)
			i += j;
		else
			++i;
	}
	return (-1);
}
