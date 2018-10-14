#include "ft_str.h"
#include "ft_mem.h"
#include "ft_types.h"
#include "ft_math.h"
#include "ft_printf.h"
#include <stdarg.h>

static int	regex_pos(t_regex_info *rgxi)
{
	const char	*str;
	int			ret;

	if (rgxi->pos)
	{
		*rgxi->pos = 0;
		str = rgxi->str;
		ret = 0;
		while ((ret = regex_exec(rgxi)) == -1)
		{
			if (!*str)
			{
				*rgxi->pos = -1;
				return (-1);
			}
			rgxi->str = ++str;
			rgxi->regex = rgxi->rgx_begin;
			++*rgxi->pos;
		}
		return (ret);
	}
	return (regex_exec(rgxi));
}

static void	regex_args(t_regex_info *rgxi, va_list vp)
{
	if (rgxi->option & RGX_RGXN)
		rgxi->rgxn = va_arg(vp, int);
	if (rgxi->option & RGX_STRN)
		rgxi->strn = va_arg(vp, int);
	if (rgxi->option & RGX_POS)
		rgxi->pos = va_arg(vp, int *);
	if (rgxi->option & RGX_VAR)
		rgxi->vars = va_arg(vp, int *);
	va_end(vp);
}

int	ft_regex(int options, const char *regex, const char *str, ...)
{
	t_regex_info	regex_info;
	int				vars[26 * 2];
	va_list			vp;

	regex_init(&regex_info, regex, str);
	ft_bzero(vars, sizeof(int) * (26 * 2));
	regex_info.vars = (int *)vars;
	regex_info.option = options;
	regex_info.param = "REGEX";
	regex_info.len_param = 5;
	va_start(vp, str);
	regex_args(&regex_info, vp);
	return (regex_pos(&regex_info));
}
