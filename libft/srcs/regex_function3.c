/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   regex_function3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 19:28:48 by mmerabet          #+#    #+#             */
/*   Updated: 2018/10/19 19:29:22 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_str.h"
#include "ft_types.h"

int			bnd_rgx(t_regex_info *rgxi, t_regex_rule *rule)
{
	(void)rule;
	if (*rule->rule == '^')
	{
		if (rgxi->str == rgxi->str_begin || !ft_isword(*(rgxi->str - 1)))
			return (0);
	}
	else if (!*rgxi->str)
		return (0);
	else if (!ft_isword(*rgxi->str))
	{
		++rgxi->str;
		return (0);
	}
	return (-1);
}

static int	get_result(t_regex_info *rgxi, t_regex_rule *rule)
{
	int		a;
	int		b;
	char	*s;

	a = regex_variable(rgxi, rule->arg + 2);
	if (!(s = ft_strnpbrk(rule->arg + 2, "+-/*%><=!", rule->len_arg - 2)))
		return (a);
	b = regex_variable(rgxi, s + 1);
	if (*s == '+' || *s == '-')
		return (*s == '+' ? (a + b) : (a - b));
	else if (*s == '*')
		return (a * b);
	else if (*s == '=' || *s == '!')
		return (*s == '=' ? (a == b) : (a != b));
	else if (*s == '>' || *s == '<')
		return (*s == '>' ? (a > b) : (a < b));
	else if (*s == '/' && b != 0)
		return (a / b);
	else if (*s == '%' && b != 0)
		return (a % b);
	return (0);
}

static int	fake_regex(t_regex_info *rgxi, t_regex_info *t, t_regex_rule *rule)
{
	char			*str;
	int				r;

	if (!(str = ft_strndup(&rule->arg[2], rule->len_arg - 2)))
		return (-1);
	*t = *rgxi;
	t->id = NULL;
	t->regex = str;
	t->rgx_begin = str;
	t->len = 0;
	t->flags = RGX_END;
	r = regex_exec(t);
	free(str);
	return (r);
}

int			expr_rgx(t_regex_info *rgxi, t_regex_rule *rule)
{
	int				r;
	int				ret;
	t_regex_info	tmp;

	r = 0;
	if (rule->len_arg <= 1)
		return (-1);
	ret = 0;
	if (rule->arg[1] == ':' || rule->arg[1] == ';')
	{
		r = fake_regex(rgxi, &tmp, rule);
		if (rule->arg[0] == '0')
			return (r == -1 ? -1 : 0);
		else if (rule->arg[1] == ';')
			ret = r;
	}
	else
		r = get_result(rgxi, rule);
	if (rule->arg[0] == '0')
		return (r ? 0 : -1);
	if (ft_isupper(rule->arg[0]))
		rgxi->vars[26 + rule->arg[0] - 65] = r;
	else if (ft_islower(rule->arg[0]))
		rgxi->vars[rule->arg[0] - 97] = r;
	return (ret);
}
