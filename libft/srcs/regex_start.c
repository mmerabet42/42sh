#include "ft_str.h"
#include "ft_mem.h"
#include "ft_printf.h"

static int	loop_return(t_regex_info *rg, t_regex_info *t,
		t_regex_rule *r, int ret)
{
	if (r->cond == RGX_MARK)
		return (0);
	if ((r->cond == RGX_GREAT && r->i <= r->l)
			|| (r->cond == RGX_EQUAL && r->i < r->l)
			|| (!r->i && ((!r->neg && ret == -1) || (r->neg && ret != -1))))
		return (-1);
	if (t->len == -1 && (!(rg->option & RGX_END) || (t->regex && *t->regex)))
		return (-1);
	return (t->len == -1 ? rg->len : rg->len + t->len);
}

static int	loop_stop(t_regex_info *rgxi, t_regex_info *tmp,
		t_regex_rule *r, int ret)
{
	int	lret;

	rgxi->len += ret;
	rgxi->str += ret;
	*tmp = *rgxi;
	tmp->option = 0;
	if (!r->cond || r->cond == RGX_MARK || r->cond == RGX_LESS
			|| (r->cond == RGX_GREAT && r->i >= r->l))
		if ((lret = regex_exec(tmp)) != -1)
			return (lret);
	if (++r->i == r->l && r->cond == RGX_EQUAL)
		return (0);
	else if (r->cond == RGX_LESS && r->i + 1 >= r->l)
		return (-1);
	return (-2);
}

static int	regex_loop(t_regex_info *rgxi, t_regex_func *func,
		t_regex_rule *rule)
{
	int				ret;
	int				lret;
	t_regex_info	tmp;

	lret = -1;
	ret = -1;
	while (*rgxi->str)
	{
		if ((ret = func->func(rgxi, rule)) == -1 && !rule->neg)
			break ;
		else if (ret != -1 && rule->neg)
			break ;
		ret = (ret == -1 ? 1 : ret);
		if ((lret = loop_stop(rgxi, &tmp, rule, ret)) != -2)
			return (lret);
		lret = -1;
	}
	tmp.len = lret;
	return (loop_return(rgxi, &tmp, rule, ret));
}

static int	regex_once(t_regex_info *rgxi, t_regex_rule *rule,
				t_regex_func *func)
{
	int	ret;

	ret = func->func(rgxi, rule);
	if (!rule->neg)
	{
		if (ret == -1 && rule->cond != RGX_MARK)
			return (-1);
		rgxi->str += (ret == -1 ? 0 : ret);
		rgxi->len += (ret == -1 ? 0 : ret);
		return (0);
	}
	else if (ret == -1)
	{
		++rgxi->str;
		++rgxi->len;
		return (0);
	}
	return (rule->cond == RGX_MARK ? 0 : -1);
}

int	regex_start(t_regex_info *rgxi, t_regex_func *func,
		t_regex_rule *rule)
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
		return (regex_loop(rgxi, func, rule));
	}
	return (regex_once(rgxi, rule, func));
}
