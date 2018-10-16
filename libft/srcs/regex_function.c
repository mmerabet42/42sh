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
	{"DEFAULT", default_rgx, 0},
	{"OTHER", other_rgx, 0},
	{"^", delim_rgx, 0},
	{"$", delim_rgx, 0},
	{"or", cond_rgx, 0},
	{"and", cond_rgx, 0},
	{"ror", cond_rgx, 0},
	{"!", regex_rgx, 0},
	{",", regex_rgx, 0},
	{";", regex_rgx, 0},
	{"R", recursive_rgx, 0},
	{"E", expr_rgx, 0},
	{"G", group_rgx, 0},
	{"LG", group_rgx, 0},

	{"upper:?[A-Z]", NULL, 0},
	{"lower:?[a-z]", NULL, 0},
	{"digit:?[0-9]", NULL, 0},
	{"alpha:?[a-zA-Z]", NULL, 0},
	{"alnum:?[a-zA-Z0-9]", NULL, 0},
	{"punct:?[?![@alnum]&?![@space]@and]", NULL, 0},
	{"word:?[a-zA-Z0-9_]", NULL, 0},
	{"space:?[ \f\n\t\r\v]", NULL, 0},
	{"int:*[@space?]?[+-@?]*[0-9]", NULL, 0},
	{"nint:?[+-@?]*[0-9]", NULL, 0},
	{"uint:*[@space?]*[0-9]", NULL, 0},
	{"getint", getint_rgx, 0},
	
	{"print", print_rgx, 0},
	{"regex", regex_rgx, 0},
	{"case", case_rgx, 0},
	{"return", print_rgx, 0},
	{"debug", debug_rgx, 0},
	{"set%", case_rgx, 0},
	{"equ:%_", NULL, 0},
	
	{"square:?[n:*[o]@E]?[m=n-2@E]*[o@=n]\n*[o*[ @=m]o\n@regex=m]*[o@=n]", NULL, 0},
	{"DQUOTE:\"*[\\\"|?![\"]@or?]\"", NULL, 0},
	{"QUOTE:'*[?![']@or?]'", NULL, 0},
	{"BSLASH:\\?", NULL, 0},
	{"BRACKET0:[*[?[@BRACKET]|?[@DQUOTE]|?[@QUOTE]|?[@BSLASH]|?![{[](){}\"'}]@or?]]", NULL, 0},
	{"BRACKET1:(*[?[@BRACKET]|?[@DQUOTE]|?[@QUOTE]|?[@BSLASH]|?![{()[]{}\"'}]@or?])", NULL, 0},
	{"BRACKET2:{*[?[@BRACKET]|?[@DQUOTE]|?[@QUOTE]|?[@BSLASH]|?![{()[]{}\"'}]@or?]}", NULL, 0},
	{"BRACKET:?[?[@BRACKET0]|?[@BRACKET1]|?[@BRACKET2]@or]", NULL, 0},
	{"INF:?[@INF]", NULL, 0},
};
static size_t		g_regex_len = (sizeof(g_regexfs) / sizeof(t_regex_func));

t_regex_func		*get_regex_func(const char *name, int len_rule, int *id)
{
	size_t	i;
	int		len;

	if (!len_rule || !name || !*name)
		return (get_regex_func("DEFAULT", 7, id));
	i = 0;
	while (i < g_regex_len)
	{
		if (!g_regexfs[i].func)
			len = ft_strchr_pos(g_regexfs[i].name, ':');
		else
			len = ft_strlen(g_regexfs[i].name);
		if (ft_strnequ(name, g_regexfs[i].name, ft_max(len_rule, len)))
		{
			if (id)
				*id = g_regexfs[i].id;
			return (&g_regexfs[i]);
		}
		++i;
	}
	return (get_regex_rule(name, len_rule, id));
}

t_regex_func		*get_regex_rule(const char *name, int len_rule, int *id)
{
	t_list			*rules;
	t_regex_func	*func;
	int				len;

	rules = NULL;
	ft_regex(RGX_GET, NULL, NULL, &rules);
	while (rules)
	{
		func = (t_regex_func *)rules->content;
		if (!func->func)
			len = ft_strchr_pos(func->name, ':');
		else
			len = ft_strlen(func->name);
		if (ft_strnequ(name, func->name, ft_max(len_rule, len)))
		{
			if (id)
				*id = func->id;
			return (func);
		}
		rules = rules->next;
	}
	return (NULL);
}
