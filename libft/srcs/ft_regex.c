#include "ft_str.h"
#include "ft_mem.h"
#include "ft_printf.h"
/*
static int	default_match(const char *regex, const char **str, const char *arg)
{
	
}
*/
static int	expanded_wildcard(t_regex_info *regex_info, int *len)
{
	int		jmp;
	char	*fct;
	int		ret;

	(void)len;
	if ((jmp = ft_strbetweenps_ext((char **)&regex_info->regex, "[?];(*)")) != -1)
	{
		fct = ft_strnchrl(regex_info->regex, '@', jmp);
		if (ft_strchr("=><]", fct[1]))
		{
			ret = 0;//default_match(regex, str, len);
		}
	
	/*
		"upper:?[A-Z]"
		"lower:?[a-z]"
		"digit:?[0-9]"
		"alpha:?{?[@upper]|?[@lower}"
		"alnum:?{?[@alpha]|?[@digit]}"
		"islower:
		
		"?[H@tolower]"

		"case:*{?[#_@islower]&?{?[#_-$_=32@expr]|?[$_-#_=32@expr}}"

		
		"*[hello@case]" "Hello" "HeLLo" "helLo"
*/
		// ft_printf("at: '%d' '%s'\n", jmp, ft_strnchrl(*regex, '@', jmp));
		// *[argument@function_name{=><}{0-9}]
		// ?[a-z] == *[a-z@=] == *[a-z@=1]
		regex_info->regex += jmp;
		return (ret);
	}
	return (-1);
}

static int	wildcard(t_regex_info *regex_info, int *len)
{
	int			ret;
	const char	*o_str;
	const char	*o_rgx;

	while (regex_info->regex[0] == '*' && regex_info->regex[1] == '*')
		++regex_info->regex;
	if (*regex_info->regex == '*')
	{
		if (*++regex_info->regex == '[' && expanded_wildcard(regex_info, len) != -1)
			return (0);
		ret = 0;
		o_str = regex_info->str;
		o_rgx = regex_info->regex;
		while (*o_str && (ret = regex_exec(regex_info)) == -1)
		{
			++o_str;
			++*len;
			regex_info->str = o_str;
			regex_info->regex = o_rgx;
		}
		regex_info->str = o_str;
		regex_info->regex = o_rgx;
		if (ret != -1)
			return (ret);
	}
	else
	{
		if (!*regex_info->str)
			return (-1);
		++regex_info->str;
		++*len;
		++regex_info->regex;
	}
	return (0);
}
int		regex_exec(t_regex_info *regex_info)
{
	int	pos;
	int	len;

	len = 0;
	while (*regex_info->regex && *regex_info->str)
	{
		if (*regex_info->regex == '*' || *regex_info->regex == '?')
		{
			if ((pos = wildcard(regex_info, &len)) == -1)
				return (-1);
			else if (pos > 0)
				return (len + pos);
			continue ;
		}
		if ((pos = ft_strpbrkl_pos(regex_info->regex, "*?")) == -1)
			pos = ft_strlen(regex_info->regex);
		if (!ft_strnequ(regex_info->regex, regex_info->str, pos))
			return (-1);
		len += pos;
		regex_info->str += pos;
		regex_info->regex += pos;
	}
	if (*regex_info->regex && !ft_strequ(regex_info->regex, "*"))
		return (-1);
	return (*regex_info->str ? -1 : len);
}

int	ft_regex(const char *regex, const char *str)
{
	t_regex_info	regex_info;

	ft_bzero(&regex_info, sizeof(regex_info));
	regex_info.str_begin = str;
	regex_info.rgx_begin = regex;
	regex_info.str = str;
	regex_info.regex = regex;
	return (regex_exec(&regex_info));
}
