#include "ft_str.h"
#include "ft_types.h"
#include "ft_math.h"
#include "ft_mem.h"
#include "ft_printf.h"

static int			default_rgx(t_regex_info *rgxi, t_regex_rule *rule)
{
	int	i;
	
	i = 0;
	while (i < rule->len_arg)
	{
		if (i + 2 < rule->len_arg && rule->arg[i + 1] == '-')
		{
			if (*rgxi->str >= rule->arg[i] && *rgxi->str <= rule->arg[i + 2])
				return (1);
			i += 2;
		}
		else if (*rgxi->str == rule->arg[i])
			return (1);
		++i;
	}
	if (!i)
		return (1);
	return (-1);
}

static int			other_rgx(t_regex_info *rgxi, t_regex_rule *rule)
{
	char			*ptr;
	t_regex_info	rgxi2;

	if (!(ptr = ft_strchr(rule->rule, ':')))
		return (-1);
	regex_init(&rgxi2, ptr + 1, rgxi->str);
	rgxi2.option = rgxi->option | RGX_END;
	rgxi2.n = rgxi->n;
	rgxi2.param = rule->arg;
	rgxi2.len_param = rule->len_arg;
	rgxi2.var0 = rgxi->var0;
	rgxi2.var1 = rgxi->var1;
	return (regex_exec(&rgxi2));
}

static int			case_rgx(t_regex_info *rgxi, t_regex_rule *rule)
{
	int	i;

	i = -1;
	while (++i < rule->len_arg && rgxi->str[i])
		if (ft_tolower(rule->arg[i]) != ft_tolower(rgxi->str[i]))
			return (-1);
	if (!rgxi->str[i] && i < rule->len_arg)
		return (-1);
	return (i);
}

static int			regex_rgx(t_regex_info *rgxi, t_regex_rule *rule)
{
	char			*str;
	int				ret;
	t_regex_info	rgxi2;

	if (ft_strnequ(rule->rule, "set%", rule->len_rule))
	{
		rgxi->param = rule->arg;
		rgxi->len_param = rule->len_arg;
		rgxi->param_i = 0;
		return (0);
	}
	if (!(str = ft_strndup(rule->arg, rule->len_arg)))
		return (-1);
	regex_init(&rgxi2, str, rgxi->str);
	rgxi2.option = RGX_END;
	rgxi2.n = -1;
	rgxi2.var0 = rgxi->var0;
	rgxi2.var1 = rgxi->var1;
	ret = regex_exec(&rgxi2);
	free(str);
	if (*rule->rule == ',' || *rule->rule == ';')
	{
		if (*rule->rule == ',' && ret != -1)
			rgxi->str += ret;
		return (ret == -1 ? -1 : 0);
	}
	else if (*rule->rule == '!')
		return (ret == -1 ? 1 : -1);
	return (ret);
}

static int			or_rgx(t_regex_info *rgxi, t_regex_rule *rule)
{
	char	*str;
	int		i;
	int		j;
	int		ret;
	int		max;
	t_regex_info	rgxi2;

	i = 0;
	max = -1;
	while (i < rule->len_arg)
	{
		if (rule->arg[i] == '|')
			++i;
		if ((j = ft_strnchr_pos(&rule->arg[i], '|', rule->len_arg - i)) == -1)
			j = rule->len_arg - i;
		if (!j || !(str = ft_strndup(&rule->arg[i], j)))
			return (!j ? 0 : -1);
		regex_init(&rgxi2, str, rgxi->str);
		rgxi2.rgx_begin = rgxi->rgx_begin;
		rgxi2.option = RGX_END;
		rgxi2.n = -1;
		rgxi2.var0 = rgxi->var0;
		rgxi2.var1 = rgxi->var1;
		ret = regex_exec(&rgxi2);
		free(str);
		if (ret != -1 && *rule->rule == 'r')
			max = ft_max(ret, max);
		else if (ret != -1)
			return (ret);
		i += j;
	}
	return (max);
}

static int			and_rgx(t_regex_info *rgxi, t_regex_rule *rule)
{
	char	*str;
	int		i;
	int		j;
	int		ret;
	t_regex_info	rgxi2;

	i = 0;
	ret = 0;
	while (i < rule->len_arg)
	{
		if (rule->arg[i] == '&')
			++i;
		if ((j = ft_strnchr_pos(&rule->arg[i], '&', rule->len_arg - i)) == -1)
			j = rule->len_arg - i;
		if (!j || !(str = ft_strndup(&rule->arg[i], j)))
			return (!j ? 0 : -1);
		regex_init(&rgxi2, str, rgxi->str);
		rgxi2.rgx_begin = rgxi->rgx_begin;
		rgxi2.option = RGX_END;
		rgxi2.n = -1;
		rgxi2.var0 = rgxi->var0;
		rgxi2.var1 = rgxi->var1;
		ret = regex_exec(&rgxi2);
		free(str);
		if (ret == -1)
			return (-1);
		i += j;
	}
	return (ret);
}

static int			get_result(int a, int b, char op)
{
	if (op == '+')
		return (a + b);
	else if (op == '-')
		return (a - b);
	else if (op == '*')
		return (a * b);
	else if (op == '=')
		return (a == b);
	else if (op == '!')
		return (a != b);
	else if (op == '>')
		return (a > b);
	else if (op == '<')
		return (a < b);
	else if (op == '/' && b != 0)
		return (a / b);
	else if (op == '%' && b != 0)
		return (a % b);
	return (0);
}

static int			expr_rgx(t_regex_info *rgxi, t_regex_rule *rule)
{
	char		t;
	const char	*op;
	char		*str;
	int			a;
	int			b;
	int			r;

	t = rule->arg[0];
	a = 0;
	b = 0;
	r = 0;
	if (rule->arg[1] == ':')
	{
		if (!(str = ft_strndup(&rule->arg[2], rule->len_arg - 2)))
			return (-1);
		r = ft_regex(str, rgxi->str, -1, RGX_END);
		free(str);
		if (t == '0')
			return (r == -1 ? -1 : 0);
	}
	else
	{
		if (ft_isdigit(rule->arg[2]))
			a = ft_atoi(&rule->arg[2]);
		else if (ft_islower(rule->arg[2]))
			a = rgxi->var0[rule->arg[2] - 97];
		else if (ft_isupper(rule->arg[2]))
			a = rgxi->var1[rule->arg[2] - 65];
		r = a;
		if ((b = ft_strnpbrk_pos(&rule->arg[2], "+-*/%=!<>", rule->len_arg)) != -1)
		{
			op = &rule->arg[2] + b;
			if (ft_isdigit(op[1]))
				b = ft_atoi(&op[1]);
			else if (ft_islower(op[1]))
				b = rgxi->var0[op[1] - 97];
			else if (ft_isupper(op[1]))
				b = rgxi->var1[op[1] - 65];
			r = get_result(a, b, *op);
		}
	}
	if (t == '0')
		return (r ? 0 : -1);
	if (ft_isupper(t))
		rgxi->var1[t - 65] = r;
	else if (ft_islower(t))
		rgxi->var0[t - 97] = r;
	return (0);
}

static int			print_rgx(t_regex_info *rgxi, t_regex_rule *rule)
{
	int	t;
	
	t = 0;
	if (ft_islower(rule->arg[0]))
		t = rgxi->var0[rule->arg[0] - 97];
	else if (ft_isupper(rule->arg[0]))
		t = rgxi->var1[rule->arg[0] - 65];
	else if (ft_isdigit(rule->arg[0]))
		t = ft_atoi(rule->arg);
	if (*rule->rule == 'r')
	{
		while (*rgxi->str)
			++rgxi->str;
		return (t);
	}
	ft_printf("var: %d\n", t);
	return (0);
}

static int			atoi_rgx(t_regex_info *rgxi, t_regex_rule *rule)
{
	int	ret;
	int	i;
	int	n;

	ret = ft_regex("?[@nint]", rgxi->str, -1, RGX_END);
	if (!ft_isalpha(rule->arg[0]))
		return (ret);
	n = (ret == -1 ? 0 : ft_atoi(rgxi->str));
	i = 0;
	while (i < rule->len_arg)
	{
		if (ft_islower(rule->arg[i]))
			rgxi->var0[rule->arg[i] - 97] = n;
		else if (ft_isupper(rule->arg[i]))
			rgxi->var1[rule->arg[i] - 65] = n;
		++i;
	}
	return (ret);
}

static int			debug_rgx(t_regex_info *rgxi, t_regex_rule *rule)
{
	ft_printf("('%.*s')str('%s') rgx('%s') begin('%s')\n",
			rule->len_arg, rule->arg, rgxi->str, rgxi->regex, rgxi->rgx_begin);
	return (0);
}

static int			recursive_rgx(t_regex_info *rgxi, t_regex_rule *rule)
{
	t_regex_info	tmp;

	regex_init(&tmp, rgxi->rgx_begin, rgxi->str);
	if (ft_isdigit(*rule->arg))
		tmp.regex += ft_atoi(rule->arg);
	tmp.var0 = rgxi->var0;
	tmp.var1 = rgxi->var1;
	tmp.option = RGX_END;
	return (regex_exec(&tmp));
}

static int			group_rgx(t_regex_info *rgxi, t_regex_rule *rule)
{
	(void)rgxi;
	(void)rule;
	return (0);
}

static t_regex_func	g_regexfs[] = {
	{"DEFAULT", default_rgx},
	{"OTHER", other_rgx},
	{"or", or_rgx},
	{"ror", or_rgx},
	{"and", and_rgx},
	{"case", case_rgx},
	{",", regex_rgx},
	{";", regex_rgx},
	{"return", print_rgx},
	{"debug", debug_rgx},
	{"R", recursive_rgx},
	{"DQUOTE:\"*[\\\"|?![\"]@or?]\"", NULL},
	{"QUOTE:'*[?![']@or?]'", NULL},
	{"BSLASH:\\?", NULL},
	{"S_BRACKET:?['\"]", NULL},
	{"G", group_rgx},
	{"LG", group_rgx},
	{"BRACKET0:[*[?[@BRACKET1]|?[@DQUOTE]|?[@QUOTE]|?[@BSLASH]|?![{[]()\"'}]|?[@BRACKET0]@or?]]", NULL},
	{"BRACKET1:(*[?[@BRACKET0]|?[@DQUOTE]|?[@QUOTE]|?[@BSLASH]|?![{()[]\"'}]|?[@BRACKET1]@or?])", NULL},
	{"BRACKETZ:?[?[@BRACKET0]|?[@BRACKET1]@or]", NULL},
	
	{"BRACKET:(*[?[@DQUOTE]|?[@QUOTE]|?[?![()]&?![@S_BRACKET]@and]|?[@BRACKET]@or?])", NULL},
	{"T:?[?[0=n!0@expr]|?[n:*[@alpha]@expr]@or]?[0=n!0@expr]*[@=n]*[@digit=n]?[?[@T]|@or]", NULL},
	{"expr", expr_rgx},
	{"print", print_rgx},
	{"regex", regex_rgx},
	{"toint", atoi_rgx},
	{"square:?[n:*[o]@expr]?[m=n-2@expr]*[o@=n]\n*[o*[ @=m]o\n@regex=m]*[o@=n]", NULL},
	{"!", regex_rgx},
	{"set%", regex_rgx},
	{"upper:?[A-Z]", NULL},
	{"lower:?[a-z]", NULL},
	{"digit:?[0-9]", NULL},
	{"alpha:?[a-zA-Z]", NULL},
	{"equ:%_", NULL},
	{"space:?[ \f\n\t\r\v]", NULL},
	{"int:?[*[ ]|@or]?[?[+-]|@or]*[0-9@>0]", NULL},
	{"nint:?[?[+-]|@or]*[0-9@>0]", NULL},
	{"uint:?[*[ ]|@or]*[0-9@>0]", NULL}
};
static size_t		g_regex_len = (sizeof(g_regexfs) / sizeof(t_regex_func));

t_regex_func		*get_regex_func(const char *name)
{
	size_t	i;
	int		len;

	if (!name || !*name)
		return (get_regex_func("DEFAULT"));
	i = 0;
	while (i < g_regex_len)
	{
		if (!g_regexfs[i].func)
			len = ft_strchr_pos(g_regexfs[i].name, ':');
		else
			len = ft_strlen(g_regexfs[i].name);
		if (ft_strnequ(name, g_regexfs[i].name, len))
			return (&g_regexfs[i]);
		++i;
	}
	return (NULL);
}
