/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   regex_function1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 19:28:22 by mmerabet          #+#    #+#             */
/*   Updated: 2018/10/19 19:28:43 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_str.h"
#include "ft_types.h"
#include "ft_math.h"
#include "ft_printf.h"

int			delim_rgx(t_regex_info *rgxi, t_regex_rule *rule)
{
	if (*rule->rule == '^' && rgxi->str == rgxi->str_begin)
		return (0);
	else if (*rule->rule == '^' && rule->rule[1] == 'n'
			&& *(rgxi->str - 1) == '\n')
		return (0);
	else if (*rule->rule == '$' && !*rgxi->str)
		return (0);
	else if (*rule->rule == '$' && rule->rule[1] == 'n'
			&& *rgxi->str == '\n')
		return (0);
	return (-1);
}

int			case_rgx(t_regex_info *rgxi, t_regex_rule *rule)
{
	int	i;

	if (ft_strnequ(rule->rule, "set%", rule->len_rule))
	{
		rgxi->param = rule->arg;
		rgxi->len_param = rule->len_arg;
		rgxi->param_i = 0;
		return (0);
	}
	i = -1;
	while (++i < rule->len_arg && rgxi->str[i])
		if (ft_tolower(rule->arg[i]) != ft_tolower(rgxi->str[i]))
			return (-1);
	if (!rgxi->str[i] && i < rule->len_arg)
		return (-1);
	return (i);
}

static int	next_op(t_regex_rule *rule, int i)
{
	int	j;
	int	pos;

	j = 0;
	while (i < rule->len_arg)
	{
		if (rule->arg[i] == (*rule->rule == 'a' ? '&' : '|'))
			return (j);
		if (rule->arg[i] == '*' || rule->arg[i] == '?')
		{
			if ((pos = regex_bracket(&rule->arg[i], NULL)) == -1)
				return (-1);
			else if (pos != -2)
			{
				j += pos - 1;
				i += pos - 1;
			}
		}
		++j;
		++i;
	}
	return (j);
}

static int	move_i(t_regex_info *rgxi, t_regex_rule *rule, int *i)
{
	char			*str;
	int				ret;
	int				j;
	t_regex_info	rgxi2;

	if (rule->arg[*i] == (*rule->rule == 'a' ? '&' : '|'))
		++*i;
	if ((j = next_op(rule, *i)) == -1)
		return (-2);
	if (!(str = ft_strndup(&rule->arg[*i], j)))
		return (-2);
	rgxi2 = *rgxi;
	rgxi2.regex = str;
	rgxi2.rgx_begin = rgxi->rgx_begin;
	rgxi2.flags = RGX_END;
	rgxi2.len = 0;
	ret = regex_exec(&rgxi2);
	free(str);
	*i += j;
	return (ret);
}

int			cond_rgx(t_regex_info *rgxi, t_regex_rule *rule)
{
	int		i;
	int		ret;
	int		max;

	i = 0;
	max = -1;
	ret = 0;
	while (i < rule->len_arg)
	{
		ret = move_i(rgxi, rule, &i);
		if (*rule->rule == 'a' && ret == -1)
			return (-1);
		else if (*rule->rule != 'a')
		{
			if (*rule->rule == 'r' && ret != -1)
				max = ft_max(ret, max);
			else if (ret != -1)
				return (ret);
		}
	}
	return (*rule->rule == 'a' ? ret : max);
}
