#include "ft_str.h"
#include "ft_types.h"

int	delim_rgx(t_regex_info *rgxi, t_regex_rule *rule)
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

int	bnd_rgx(t_regex_info *rgxi, t_regex_rule *rule)
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

int	case_rgx(t_regex_info *rgxi, t_regex_rule *rule)
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
