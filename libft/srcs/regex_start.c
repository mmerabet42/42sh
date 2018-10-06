#include "ft_str.h"
#include "ft_mem.h"
#include "ft_printf.h"

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
	ret = -1;
	ft_bzero(&tmp, sizeof(t_regex_info));
	while (*rgxi->str && (((ret = func->func(rgxi, rule)) != -1 && !rule->neg) || (ret == -1 && rule->neg)))
	{
		ret = (ret == -1 ? 1 : ret);
		len += ret;
		rgxi->str += ret;
		tmp = *rgxi;
		tmp.option = 0;
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
	if ((ret == -1 && !rule->neg && !i) || (ret != -1 && rule->neg && !i)
			|| (lret == -1 && (!(rgxi->option & RGX_END) || (tmp.regex && *tmp.regex))))
		return (-1);
	return (lret == -1 ? len : len + lret);
}

static int	regex_once(t_regex_info *rgxi, t_regex_rule *rule,
				t_regex_func *func, int *len)
{
	int	ret;

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
	return (rule->cond == RGX_MARK ? 0 : -1);
}

int	regex_start(t_regex_info *rgxi, t_regex_func *func,
		t_regex_rule *rule, int *len)
{
	if (!func)
		return (-1);
	else if (!func->func)
	{
		rule->rule = func->name;
		rule->len_rule = ft_strlen(rule->rule);
		if (!(func = get_regex_func("OTHER", 5)))
			return (-1);
	}
	if (rule->type == '*')
	{
		if (rule->cond == RGX_EQUAL && rule->l <= 0)
			return (0);
		else if (rule->cond == RGX_LESS && rule->l <= 1)
			return (0);
		return (regex_loop(rgxi, func, rule, len));
	}
	return (regex_once(rgxi, rule, func, len));
}
