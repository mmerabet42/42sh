#include "ft_str.h"
#include "ft_printf.h"

int	delim_rgx(t_regex_info *rgxi, t_regex_rule *rule)
{
	if (*rule->rule == '^' && rgxi->str == rgxi->str_begin)
		return (0);
	else if (*rule->rule == '$')
	{
		ft_printf("hellocidshc\n");
		if (!*rgxi->str)
			return (0);
		return (-1);
	}
	return (-1);
}
