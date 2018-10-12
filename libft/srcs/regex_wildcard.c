#include "ft_str.h"
#include "ft_mem.h"
#include "ft_types.h"
#include "ft_printf.h"

static void	fill_info(t_regex_info *rgxi, t_regex_rule *r, int jmp, int ret)
{
	if (r->len_arg < 0)
		r->len_arg = jmp - ret - (ret - (r->neg ? 2 : 1)) - r->cond;
	else
	{
		r->len_arg -= r->cond;
		r->rule = r->arg + r->len_arg + 1;
		r->len_rule = ft_strpbrk_pos(r->rule, "?=><}]");
		r->cond = 0;
		if (r->rule[r->len_rule] == '=')
			r->cond = RGX_EQUAL;
		else if (r->rule[r->len_rule] == '>' || r->rule[r->len_rule] == '<')
			r->cond = (r->rule[r->len_rule] == '>' ? RGX_GREAT : RGX_LESS);
		else if (r->rule[r->len_rule] == '?')
			r->cond = RGX_MARK;
		if (r->rule[r->len_rule] != ']' && r->rule[r->len_rule] != '}')
		{
			r->l = regex_variable(rgxi, r->rule + r->len_rule + 1);
		/*	if (ft_isdigit(r->rule[r->len_rule + 1]))
				r->l = ft_atoi(&r->rule[r->len_rule + 1]);
			else if (ft_islower(r->rule[r->len_rule + 1]))
				r->l = rgxi->var0[r->rule[r->len_rule + 1] - 97];
			else if (ft_isupper(r->rule[r->len_rule + 1]))
				r->l = rgxi->var1[r->rule[r->len_rule + 1] - 65];*/
		}
	}
}

static int	expanded_wildcard(t_regex_info *rgxi, char type, int neg)
{
	int				jmp;
	int				ret;
	t_regex_rule	rule;

	if ((jmp = regex_bracket(rgxi->regex, &ret)) != -1)
	{
		ft_bzero(&rule, sizeof(t_regex_rule));
		rule.type = type;
		rule.neg = neg;
		rule.len_arg = ft_strnrchr_pos(rgxi->regex + ret, '@', jmp - ret);
		if (*(rule.arg = rgxi->regex + ret) == '\\'
				&& (rule.arg[1] == '{' || rule.arg[1] == '\\'))
		{
			rule.cond = 1;
			++rule.arg;
		}
		rgxi->regex += jmp;
		fill_info(rgxi, &rule, jmp, ret);
		ret = regex_start(rgxi, get_regex_func(rule.rule, rule.len_rule),
				&rule);
		return (ret);
	}
	return (-1);
}

static int	regex_mark(t_regex_info *rgxi)
{
	int	neg;

	neg = 0;
	if (*++rgxi->regex == '!' && *(rgxi->regex + 1) == '[')
	{
		++rgxi->regex;
		neg = 1;
	}
	if (*rgxi->regex == '[' && (rgxi->regex -= (neg ? 2 : 1)))
		return (expanded_wildcard(rgxi, '?', neg));
	if (!*rgxi->str)
		return (-1);
	++rgxi->str;
	++rgxi->len;
	return (0);
}

int		regex_wildcard(t_regex_info *rgxi)
{
	int			ret;
	int			neg;
	const char	*o_str;
	const char	*o_rgx;

	if (!(neg = 0) && *rgxi->regex == '*')
	{
		if (*++rgxi->regex == '!' && rgxi->regex[1] == '[' && ++rgxi->regex)
			neg = 1;
		if (*rgxi->regex == '[' && (rgxi->regex -= (neg ? 2 : 1)))
			return (expanded_wildcard(rgxi, '*', neg));
		o_str = rgxi->str;
		if (!*(o_rgx = rgxi->regex))
			return (rgxi->len + ft_strlen(rgxi->str));
		ret = 0;
		while (*o_str && (ret = regex_exec(rgxi)) == -1)
		{
			++rgxi->len;
			rgxi->str = ++o_str;
			rgxi->regex = o_rgx;
		}
		rgxi->str = o_str;
		rgxi->regex = o_rgx;
		return (ret != -1 ? ret : 0);
	}
	return (regex_mark(rgxi));
}
