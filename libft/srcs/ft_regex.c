#include "ft_str.h"
#include "ft_mem.h"
#include "ft_types.h"
#include "ft_math.h"
#include "ft_printf.h"
#include <stdarg.h>

static int	regex_modulus(t_regex_info *rgxi)
{
	int	v;
	
	if (rgxi->param && (*rgxi->regex == '.' || *rgxi->regex == '+'))
	{
		if (rgxi->param_i >= rgxi->len_param)
			return (0);
		v = -1;
		if (*rgxi->str == rgxi->param[rgxi->param_i])
			v = 1;
		if (*rgxi->regex == '+')
			++rgxi->param_i;
		++rgxi->regex;
		return (v);
	}
	else if (rgxi->param && *rgxi->regex == '_' && ++rgxi->regex)
	{
		if (ft_strnequ(rgxi->str, rgxi->param, rgxi->len_param))
			return (rgxi->len_param);
	}
	else if (*rgxi->str == '%')
		return (1);
	return (-1);
}

static int	special_char(char c, t_regex_info *rgxi, int *len)
{
	int	pos;

	if (c == '%')
	{
		++rgxi->regex;
		if ((pos = regex_modulus(rgxi)) == -1)
			return (-1);
		*len += pos;
		rgxi->str += pos;
		return (0);
	}
	else if (c == '*' || c == '?')
	{
		if ((pos = regex_wildcard(rgxi, len)) == -1)
			return (-1);
		else if (pos > 0)
			return (*len + pos);
		return (0);
	}
	return (-2);
}

int		regex_exec(t_regex_info *regex_info)
{
	int	pos;
	int	len;

	len = 0;
	while (*regex_info->regex)
	{
		if ((pos = special_char(*regex_info->regex, regex_info, &len)) == 0)
			continue;
		else if (pos != -2)
			return (pos);
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
	regex_info.param = "REGEX";
	regex_info.len_param = 5;
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
