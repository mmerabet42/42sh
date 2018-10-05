#include "ft_str.h"
#include "ft_mem.h"
#include "ft_types.h"
#include "ft_math.h"
#include "ft_printf.h"
#include <stdarg.h>

static int	regex_loop(t_regex_info *rgxi, t_regex_func *func, t_regex_rule *rule, int *len2)
{
	int				len;
	int				ret;
	int				lret;
	int				i;
	t_regex_info	tmp;

	i = 0;
	len = 0;
	lret = -1;
	ft_bzero(&tmp, sizeof(t_regex_info));
	if (rule->cond == RGX_EQUAL && rule->l <= 0)
		return (0);
	else if (rule->cond == RGX_LESS && rule->l <= 1)
		return (0);
	while (*rgxi->str && (((ret = func->func(rgxi, rule)) != -1 && !rule->neg) || (ret == -1 && rule->neg)))
	{
		ret = (ret == -1 ? 1 : ret);
		len += ret;
		rgxi->str += ret;
		tmp.str_begin = rgxi->str_begin;
		tmp.rgx_begin = rgxi->rgx_begin;
		tmp.str = rgxi->str;
		tmp.regex = rgxi->regex;
		tmp.len = rgxi->len + len;
		tmp.n = rgxi->n;
		tmp.var0 = rgxi->var0;
		tmp.var1 = rgxi->var1;
		if (!rule->cond || rule->cond == RGX_MARK || rule->cond == RGX_LESS
				|| (rule->cond == RGX_GREAT && i >= rule->l))
			if ((lret = regex_exec(&tmp)) != -1)
				return (len + lret);
		if (++i == rule->l && rule->cond == RGX_EQUAL)
		{
			*len2 += len;
			return (0);
		}
		else if (rule->cond == RGX_LESS && i + 1 >= rule->l)
			return (-1);
	}
	if (rule->cond == RGX_MARK)
	{
		*len2 += len;
		return (0);
	}
	if ((rule->cond == RGX_GREAT && i <= rule->l) || (rule->cond == RGX_EQUAL && i < rule->l))
		return (-1);
	if ((ret == -1 && !i) || (lret == -1 && !(rgxi->option & RGX_END)))
		return (-1);
	if (lret == -1)
		return (len);
	return (len + lret);
}

static int	regex_start(t_regex_info *rgxi, t_regex_func *func, t_regex_rule *rule, int *len)
{
	int	ret;

	if (!func)
		return (-1);
	else if (!func->func)
	{
		rule->rule = func->name;
		rule->len_rule = ft_strlen(rule->rule);
		if (!(func = get_regex_func("OTHER")))
			return (-1);
	}
	if (rule->type == '*')
		return (regex_loop(rgxi, func, rule, len));
	ret = func->func(rgxi, rule);
	if (!rule->neg)
	{
		if (ret == -1 && rule->cond != RGX_MARK)
			return (-1);
		rgxi->str += (ret == -1 ? 0 : ret);
		*len += (ret == -1 ? 0 : ret);
		return (0);
	}
	else if (ret == -1)
	{
		++rgxi->str;
		++*len;
		return (0);
	}
	else if (rule->cond == RGX_MARK)
		return (0);
	return (-1);
}

static int	expanded_wildcard(t_regex_info *rgxi, char type, int *len, int neg)
{
	int	jmp;
	int	ret;
	t_regex_rule	rule;

	rgxi->regex -= (neg ? 2 : 1);
	if ((jmp = regex_bracket(rgxi->regex)) != -1)
	{
		ft_bzero(&rule, sizeof(t_regex_rule));
		rule.type = type;
		rule.neg = neg;
		neg = (neg ? 3 : 2);
		rule.arg = rgxi->regex + neg;
		ret = 1;
		if (rgxi->regex[neg] == '{' && (ret = 2))
			rule.arg = rgxi->regex + (neg + 1);
		else if (rgxi->regex[neg] == '\\' && rgxi->regex[neg + 1] == '{')
			rule.arg = rgxi->regex + ++neg;
		rule.len_arg = ft_strnrchr_pos(rgxi->regex, '@', jmp);
		rgxi->regex += jmp;
		if (rule.len_arg == -1)
		{
			rule.len_arg = (int)(rgxi->regex - rule.arg) - ret;
			rule.rule = "";
			rule.len_rule = 0;
		}
		else
		{
			rule.len_arg -= neg + ret - 1;
			rule.rule = rule.arg + rule.len_arg + 1;
			rule.len_rule = ft_strpbrk_pos(rule.rule, "?=><]");
			if (rule.rule[rule.len_rule] == '=')
				rule.cond = RGX_EQUAL;
			else if (rule.rule[rule.len_rule] == '<')
				rule.cond = RGX_LESS;
			else if (rule.rule[rule.len_rule] == '>')
				rule.cond = RGX_GREAT;
			else if (rule.rule[rule.len_rule] == '?')
				rule.cond = RGX_MARK;
			if (rule.rule[rule.len_rule] != ']')
			{
				if (ft_isdigit(rule.rule[rule.len_rule + 1]))
					rule.l = ft_atoi(&rule.rule[rule.len_rule + 1]);
				else if (ft_islower(rule.rule[rule.len_rule + 1]))
					rule.l = rgxi->var0[rule.rule[rule.len_rule + 1] - 97];
				else if (ft_isupper(rule.rule[rule.len_rule + 1]))
					rule.l = rgxi->var1[rule.rule[rule.len_rule + 1] - 65];
			}
		}
		ret = regex_start(rgxi, get_regex_func(rule.rule), &rule, len);
		return (ret);
	}
	return (-1);
}

static int	wildcard(t_regex_info *rgxi, int *len)
{
	int			ret;
	int			neg;
	const char	*o_str;
	const char	*o_rgx;

	neg = 0;
	if (*rgxi->regex == '*')
	{
		if (*++rgxi->regex == '!' && *(rgxi->regex + 1) == '[')
		{
			++rgxi->regex;
			neg = 1;
		}
		if (*rgxi->regex == '[')
			return (expanded_wildcard(rgxi, '*', len, neg));
		ret = 0;
		o_str = rgxi->str;
		o_rgx = rgxi->regex;
		if (!*rgxi->regex)
			return (ft_strlen(rgxi->str));
		while (*o_str && (ret = regex_exec(rgxi)) == -1)
		{
			++o_str;
			++*len;
			rgxi->str = o_str;
			rgxi->regex = o_rgx;
		}
		rgxi->str = o_str;
		rgxi->regex = o_rgx;
		if (ret != -1)
			return (ret);
	}
	else
	{
		if (*++rgxi->regex == '!' && *(rgxi->regex + 1) == '[')
		{
			++rgxi->regex;
			neg = 1;
		}
		if (*rgxi->regex == '[')
			return (expanded_wildcard(rgxi, '?', len, neg));
		if (!*rgxi->str)
			return (-1);
		++rgxi->str;
		++*len;
	}
	return (0);
}

static int	regex_modulus(t_regex_info *regex_info)
{
	++regex_info->regex;
	if (!regex_info->param)
	{
		if (*regex_info->str == '%')
			return (1);
	}
	else if (*regex_info->regex == '.')
	{
		++regex_info->regex;
		if (!regex_info->param[regex_info->param_i])
			return (0);
		if (*regex_info->str == regex_info->param[regex_info->param_i])
			return (1);
	}
	else if (*regex_info->regex == '+')
	{
		++regex_info->regex;
		if (!regex_info->param[regex_info->param_i])
			return (0);
		if (*regex_info->str == regex_info->param[regex_info->param_i++])
			return (1);
	}
	else if (*regex_info->regex == '_')
	{
		++regex_info->regex;
		if (ft_strnequ(regex_info->str, regex_info->param, regex_info->len_param))
			return (regex_info->len_param);
	}
	else if (*regex_info->str == '%')
		return (1);
	return (-1);
}

int		regex_exec(t_regex_info *regex_info)
{
	int	pos;
	int	len;
	char	c;

	len = 0;
	while (*regex_info->regex)
	{
		c = *regex_info->regex;
		if (c == '%')
		{
			if ((pos = regex_modulus(regex_info)) == -1)
				return (-1);
			len += pos;
			regex_info->str += pos;
			continue ;
		}
		else if (c == '*' || c == '?')
		{
			if ((pos = wildcard(regex_info, &len)) == -1)
				return (-1);
			else if (pos > 0)
				return (len + pos);
			continue ;
		}
		if ((pos = ft_strpbrk_pos(regex_info->regex, "*?")) == -1)
			pos = ft_strlen(regex_info->regex);
		if (!ft_strnequ(regex_info->regex, regex_info->str, pos))
			return (-1);
		len += pos;
		regex_info->str += pos;
		regex_info->regex += pos;
	}
	if ((regex_info->option & RGX_END) && *regex_info->str)
		return (len);
	if (*regex_info->regex && !ft_strequ(regex_info->regex, "*"))
		return (-1);
	return (*regex_info->str ? -1 : len);
}

void	regex_init(t_regex_info *regex_info, const char *regex, const char *str)
{
	ft_bzero(regex_info, sizeof(t_regex_info));
	regex_info->str_begin = str;
	regex_info->rgx_begin = regex;
	regex_info->str = str;
	regex_info->regex = regex;
	regex_info->n = -1;
}

int	ft_regex(const char *regex, const char *str, int n, int opt)
{
	t_regex_info	regex_info;
	int				var0[26];
	int				var1[26];

	regex_init(&regex_info, regex, str);
	ft_bzero(var0, sizeof(int) * 26);
	ft_bzero(var1, sizeof(int) * 26);
	regex_info.option = opt;
	regex_info.n = n;
	regex_info.var0 = (int *)var0;
	regex_info.var1 = (int *)var1;
	return (regex_exec(&regex_info));
}

int	ft_lregex(int options, const char *a, const char *b, ...)
{
	t_regex_info	regex_info;
	int				var0[26];
	int				var1[26];
//	va_list			vp;

	/*
	RGX_STRN,	RGX_RGXN,	RGX_POS
	int strn,	int rgxn,	int *pos
							RGX_MATCHES
							t_list **matches
	RGX_ADDRULE
	t_regex_funcptr *func
	RGX_GETRULES
	t_list **rules;
	RGX_CLEANRULES
	*/

	regex_init(&regex_info, a, b);
	ft_bzero(var0, sizeof(int) * 26);
	ft_bzero(var1, sizeof(int) * 26);
	regex_info.option = options;
	regex_info.var0 = (int *)var0;
	regex_info.var1 = (int *)var1;
	return (regex_exec(&regex_info));
}
