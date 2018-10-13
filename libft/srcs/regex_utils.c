/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   regex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/12 20:05:27 by mmerabet          #+#    #+#             */
/*   Updated: 2018/10/13 20:30:28 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_str.h"
#include "ft_mem.h"
#include "ft_types.h"

void	regex_init(t_regex_info *regex_info, const char *regex, const char *str)
{
	ft_bzero(regex_info, sizeof(t_regex_info));
	regex_info->str_begin = str;
	regex_info->rgx_begin = regex;
	regex_info->str = str;
	regex_info->regex = regex;
	regex_info->n = -1;
}

int	regex_variable(t_regex_info *rgxi, const char *s)
{
	if (ft_isdigit(*s))
		return (ft_atoi(s));
	else if (ft_islower(*s))
		return (rgxi->vars[*s - 97]);
	else if (ft_isupper(*s))
		return (rgxi->vars[26 + *s - 65]);
	return (0);
}
