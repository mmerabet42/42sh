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
	rgxi2.param = rule->arg;
	rgxi2.len_param = rule->len_arg;
	rgxi2.vars = rgxi->vars;
	return (regex_exec(&rgxi2));
}

static int			case_rgx(t_regex_info *rgxi, t_regex_rule *rule)
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

static int			regex_rgx(t_regex_info *rgxi, t_regex_rule *rule)
{
	char			*str;
	int				ret;
	t_regex_info	rgxi2;

	if (!(str = ft_strndup(rule->arg, rule->len_arg)))
		return (-1);
	regex_init(&rgxi2, str, rgxi->str);
	rgxi2.option = RGX_END;
	rgxi2.vars = rgxi->vars;
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

static t_regex_func	g_regexfs[] = {
	{"DEFAULT", default_rgx},
	{"OTHER", other_rgx},
	{"^", delim_rgx},
	{"$", delim_rgx},
	{"or", cond_rgx},
	{"and", cond_rgx},
	{"ror", cond_rgx},
	{"!", regex_rgx},
	{",", regex_rgx},
	{";", regex_rgx},
	{"R", recursive_rgx},
	{"E", expr_rgx},
	{"G", group_rgx},
	{"LG", group_rgx},

	{"upper:?[A-Z]", NULL},
	{"lower:?[a-z]", NULL},
	{"digit:?[0-9]", NULL},
	{"alpha:?[a-zA-Z]", NULL},
	{"alnum:?[a-zA-Z0-9]", NULL},
	{"punct:?[?![@alnum]&?![@space]@and]", NULL},
	{"space:?[ \f\n\t\r\v]", NULL},
	{"int:*[@space?]?[+-@?]*[0-9]", NULL},
	{"nint:?[+-@?]*[0-9]", NULL},
	{"uint:*[@space?]*[0-9]", NULL},
	{"getint", getint_rgx},
	
	{"print", print_rgx},
	{"regex", regex_rgx},
	{"case", case_rgx},
	{"return", print_rgx},
	{"debug", debug_rgx},
	{"set%", case_rgx},
	{"equ:%_", NULL},
	
	{"square:?[n:*[o]@E]?[m=n-2@E]*[o@=n]\n*[o*[ @=m]o\n@regex=m]*[o@=n]", NULL},
	{"DQUOTE:\"*[\\\"|?![\"]@or?]\"", NULL},
	{"QUOTE:'*[?![']@or?]'", NULL},
	{"BSLASH:\\?", NULL},
	{"BRACKET0:[*[?[@BRACKET1]|?[@DQUOTE]|?[@QUOTE]|?[@BSLASH]|?![{[]()\"'}]|?[@BRACKET0]@or?]]", NULL},
	{"BRACKET1:(*[?[@BRACKET0]|?[@DQUOTE]|?[@QUOTE]|?[@BSLASH]|?![{()[]\"'}]|?[@BRACKET1]@or?])", NULL},
	{"BRACKET:?[?[@BRACKET0]|?[@BRACKET1]@or]", NULL},
	{"INF:?[@INF]", NULL},
};
static size_t		g_regex_len = (sizeof(g_regexfs) / sizeof(t_regex_func));

t_regex_func		*get_regex_func(const char *name, int len_rule)
{
	size_t	i;
	int		len;

	if (!len_rule || !name || !*name)
		return (get_regex_func("DEFAULT", 7));
	i = 0;
	while (i < g_regex_len)
	{
		if (!g_regexfs[i].func)
			len = ft_strchr_pos(g_regexfs[i].name, ':');
		else
			len = ft_strlen(g_regexfs[i].name);
		if (ft_strnequ(name, g_regexfs[i].name, ft_max(len_rule, len)))
			return (&g_regexfs[i]);
		++i;
	}
	return (NULL);
}
