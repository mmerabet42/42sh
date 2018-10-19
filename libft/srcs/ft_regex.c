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
				return (-1);
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
	t_regex_match	umatch;
	const char		*str;
	int				global_pos;
	int				zero;
	int				i;

	rgxi->pos = &match.pos;
	rgxi->id = &match.id;
	str = rgxi->str;
	head = NULL;
	i = 0;
	global_pos = 0;
	zero = 0;
	while ((match.len = regex_pos(rgxi)) != -1)
	{
		if ((rgxi->flags & RGX_UGLOBAL) && match.pos)
		{
			umatch.pos = global_pos - (zero ? 1: 0);
			umatch.len = match.pos + (zero ? 1 : 0);
			umatch.str = str + umatch.pos;
			umatch.id = -1;
			ft_lstpush_p(&head, ft_lstnew(&umatch, sizeof(t_regex_match)));
		}
		match.pos += global_pos;
		if (rgxi->flags & RGX_GLOBAL)
		{
			match.str = str + match.pos;
			ft_lstpush_p(&head, ft_lstnew(&match, sizeof(t_regex_match)));
			match.id = 0;
		}
		++i;
		zero = (!match.len ? 1 : 0);
		global_pos = match.pos + (!match.len ? 1 : match.len);
		if (!*(str + match.pos) || !*(rgxi->str = str + global_pos))
			break ;
		rgxi->regex = rgxi->rgx_begin;
		rgxi->len = 0;
	}
	if ((rgxi->flags & RGX_UGLOBAL) && match.len == -1)
	{
		umatch.pos = global_pos;
		umatch.len = match.pos;
		umatch.str = str + global_pos;
		umatch.id = -1;
		ft_lstpush_p(&head, ft_lstnew(&umatch, sizeof(t_regex_match)));
	}
	*rgxi->matches = head;
	return (i);
}

static void	get_args(t_regex_info *rgxi, va_list vp)
{
	if (rgxi->flags & RGX_RGXN)
		rgxi->rgxn = va_arg(vp, int);
	if (rgxi->flags & RGX_STRN)
		rgxi->strn = va_arg(vp, int);
	if (rgxi->flags & (RGX_GLOBAL | RGX_UGLOBAL))
		rgxi->matches = (t_list **)va_arg(vp, t_list **);
	else
	{
		if (rgxi->flags & RGX_POS)
			rgxi->pos = va_arg(vp, int *);
		if (rgxi->flags & RGX_ID)
			rgxi->id = va_arg(vp, int *);
	}
	if (rgxi->flags & RGX_VAR)
		rgxi->vars = va_arg(vp, int *);
	va_end(vp);
}

int			ft_regex(int flags, const char *regex, const char *str, ...)
{
	static t_list	*rules;
	t_regex_info	regex_info;
	int				vars[52];
	va_list			vp;

	va_start(vp, str);
	if (flags & (RGX_ADD | RGX_GET | RGX_CLEAN | RGX_FREE))
		return (manage_rules(str, &rules, flags, vp));
	regex_init(&regex_info, regex, str);
	ft_bzero(vars, sizeof(int) * (52));
	regex_info.vars = (int *)vars;
	regex_info.flags = flags;
	regex_info.param = "REGEX";
	regex_info.len_param = 5;
	get_args(&regex_info, vp);
	if (regex_info.matches)
	{
		regex_info.flags |= (RGX_POS | RGX_END | RGX_ID);
		return (get_matches(&regex_info));
	}
	return (regex_pos(&regex_info));
}

void		ft_print_matches(const char *str, t_list *matches)
{
	int				i;
	int				n;
	const char		*color;
	t_regex_match	*m;

	if (!matches)
		return ;
	i = 0;
	n = 0;
	ft_printf("%#{magenta}{%{0}");
	while (matches)
	{
		m = (t_regex_match *)matches->content;
		color = (n ? "lcyan" : "lblue");
		if (m->id == -1)
			color = "white";
		ft_printf("%{white}%#{black}%.*s%{black}%#{%s}%.*s%#{lred}%d%{0}",
				m->pos - i, str + i, color, m->len, m->str, m->id);
		n = !n;
		i = m->pos + m->len;
		if (!(matches = matches->next))
			ft_printf("%{white}%#{black}%s%{0}", str + i);
	}
	ft_printf("%#{magenta}}%{0}\n");
}
