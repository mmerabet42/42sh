#include "ft_str.h"
#include "ft_types.h"
#include "ft_math.h"
#include "ft_printf.h"

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

int			expr_rgx(t_regex_info *rgxi, t_regex_rule *rule)
{
	char			*str;
	int				r;
	t_regex_info	tmp;

	r = 0;
	if (rule->len_arg <= 1)
		return (-1);
	if (rule->arg[1] == ':')
	{
		if (!(str = ft_strndup(&rule->arg[2], rule->len_arg - 2)))
			return (-1);
		tmp = *rgxi;
		tmp.regex = str;
		tmp.rgx_begin = str;
		tmp.len = 0;
		tmp.option = RGX_END;
		r = regex_exec(&tmp);
		free(str);
		if (rule->arg[0] == '0')
			return (r == -1 ? -1 : 0);
	}
	else
		r = get_result(rgxi, rule);
	if (rule->arg[0] == '0')
		return (r ? 0 : -1);
	if (ft_isupper(rule->arg[0]))
		rgxi->vars[26 + rule->arg[0] - 65] = r;
	else if (ft_islower(rule->arg[0]))
		rgxi->vars[rule->arg[0] - 97] = r;
	return (0);
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
	rgxi2.option = RGX_END;
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
