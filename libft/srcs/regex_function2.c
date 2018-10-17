#include "ft_str.h"
#include "ft_types.h"
#include "ft_printf.h"

int	print_rgx(t_regex_info *rgxi, t_regex_rule *rule)
{
	int	t;

	t = 0;
	if (ft_islower(rule->arg[0]))
		t = rgxi->vars[rule->arg[0] - 97];
	else if (ft_isupper(rule->arg[0]))
		t = rgxi->vars[26 + rule->arg[0] - 65];
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

int	getint_rgx(t_regex_info *rgxi, t_regex_rule *rule)
{
	int	ret;
	int	i;
	int	n;

	ret = ft_regex(RGX_END, "?[@nint]", rgxi->str);
	if (!ft_isalpha(rule->arg[0]))
		return (ret);
	n = (ret == -1 ? 0 : ft_atoi(rgxi->str));
	i = 0;
	while (i < rule->len_arg)
	{
		if (ft_islower(rule->arg[i]))
			rgxi->vars[rule->arg[i] - 97] = n;
		else if (ft_isupper(rule->arg[i]))
			rgxi->vars[26 + rule->arg[i] - 65] = n;
		++i;
	}
	return (ret);
}

int	debug_rgx(t_regex_info *rgxi, t_regex_rule *rule)
{
	ft_printf("('%.*s')str('%s') rgx('%s') begin('%s')\n",
			rule->len_arg, rule->arg, rgxi->str, rgxi->regex, rgxi->rgx_begin);
	return (0);
}

int	recursive_rgx(t_regex_info *rgxi, t_regex_rule *rule)
{
	t_regex_info	tmp;

	regex_init(&tmp, rgxi->rgx_begin, rgxi->str);
	if (ft_isdigit(*rule->arg))
		tmp.regex += ft_atoi(rule->arg);
	tmp.vars = rgxi->vars;
	tmp.option = RGX_END;
	return (regex_exec(&tmp));
}
