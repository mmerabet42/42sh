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

static int	special_char(char c, t_regex_info *rgxi)
{
	int	pos;

	if (c == '%')
	{
		++rgxi->regex;
		if ((pos = regex_modulus(rgxi)) == -1)
			return (-1);
		rgxi->len += pos;
		rgxi->str += pos;
		return (0);
	}
	else if (c == '*' || c == '?')
	{
		pos = regex_wildcard(rgxi);
		if (pos == -1)
			return (-1);
		else if (pos > 0)
			return (pos);
		return (0);
	}
	return (-2);
}

static int	regex_equ(t_regex_info *rgxi)
{
	int	i;

	i = 0;
	while (rgxi->regex[i] && rgxi->str[i])
	{
		if (rgxi->regex[i] == '*' || rgxi->regex[i] == '?')
			return (i);
		else if (rgxi->regex[i] != rgxi->str[i])
			return (-1);
		++i;
	}
	if (!rgxi->str[i] && rgxi->regex[i])
		return (-1);
	else if (!rgxi->regex[i] && rgxi->str[i] && !(rgxi->option & RGX_END))
		return (-1);
	return (i);
}

int		regex_exec(t_regex_info *regex_info)
{
	int	pos;

	while (*regex_info->regex)
	{
		if ((pos = special_char(*regex_info->regex, regex_info)) == 0)
			continue;
		else if (pos != -2)
			return (pos);
		if ((pos = regex_equ(regex_info)) == -1)
			return (-1);
		regex_info->len += pos;
		regex_info->str += pos;
		regex_info->regex += pos;
	}
	if ((regex_info->option & RGX_END) && *regex_info->str)
		return (regex_info->len);
	if (*regex_info->regex && !ft_strequ(regex_info->regex, "*"))
		return (-1);
	return (*regex_info->str ? -1 : regex_info->len);
}

int	ft_regex(int options, const char *regex, const char *str, ...)
{
	t_regex_info	regex_info;
	int				vars[26 * 2];
//	int				var1[26];

	regex_init(&regex_info, regex, str);
	ft_bzero(vars, sizeof(int) * (26 * 2));
//	ft_bzero(var1, sizeof(int) * 26);
	regex_info.option = options;
	regex_info.vars = (int *)vars;
//	regex_info.var1 = (int *)var1;
	regex_info.param = "REGEX";
	regex_info.len_param = 5;
	return (regex_exec(&regex_info));
}
