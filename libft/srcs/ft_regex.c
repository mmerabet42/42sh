#include "ft_str.h"
#include "ft_mem.h"
#include "ft_types.h"
#include "ft_math.h"
#include "ft_printf.h"
#include "ft_list.h"
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
			rgxi->len = 0;
			++*rgxi->pos;
		}
		return (ret);
	}
	return (regex_exec(rgxi));
}

static int	get_matches(t_regex_info *rgxi)
{
	t_list			*head;
	t_regex_match	match;
	const char		*str;
	int				global_pos;
	int				i;

	rgxi->option &= ~RGX_MATCHES;
	rgxi->option |= RGX_POS;
	rgxi->option |= RGX_END;
	rgxi->pos = &match.pos;
	str = rgxi->str;
	head = NULL;
	i = 0;
	global_pos = 0;
	while ((match.len = regex_pos(rgxi)) != -1)
	{
		match.pos += global_pos;
		match.str = str + match.pos;
		ft_lstpush_p(&head, ft_lstnew(&match, sizeof(t_regex_match)));
		global_pos = match.pos + match.len;
		rgxi->str = str + global_pos;
		rgxi->regex = rgxi->rgx_begin;
		rgxi->len = 0;
		++i;
	}
	ft_lstpush_p(rgxi->matches, head);
	return (i);
}

static void	get_args(t_regex_info *rgxi, va_list vp)
{
	if (rgxi->option & RGX_RGXN)
		rgxi->rgxn = va_arg(vp, int);
	if (rgxi->option & RGX_STRN)
		rgxi->strn = va_arg(vp, int);
	if (rgxi->option & RGX_MATCHES)
		rgxi->matches = (t_list **)va_arg(vp, t_list **);
	else if (rgxi->option & RGX_POS)
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
	get_args(&regex_info, vp);
	if (regex_info.matches)
		return (get_matches(&regex_info));
	return (regex_pos(&regex_info));
}

void	ft_print_matches(const char *str, t_list *matches)
{
	int				i;
	int				n;
	t_regex_match	*m;

	if (!matches)
		return ;
	i = 0;
	n = 0;
	ft_printf("%#{magenta}{%{0}");
	while (matches)
	{
		m = (t_regex_match *)matches->content;
		ft_printf("%{white}%#{black}%.*s%{black}%#{%s}%.*s%{0}",
				m->pos - i, str + i, (n ? "lcyan" : "lblue"), m->len, m->str);
		n = !n;
		i = m->pos + m->len;
		if (!(matches = matches->next))
			ft_printf("%{white}%#{black}%s%{0}", str + i);
	}
	ft_printf("%#{magenta}}%{0}\n");
}
