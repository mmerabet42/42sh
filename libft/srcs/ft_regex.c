#include "ft_str.h"
#include "ft_printf.h"

static int	default_match(const char *regex, const char **str, const char *arg)
{
	
}

static int	expanded_wildcard(const char **regex, const char **str, int *len)
{
	int		jmp;
	char	*fct;
	int		ret;

	(void)str;
	(void)len;
	if ((jmp = ft_strbetweenps_ext((char **)regex, "[?];{?}")) != -1)
	{
		fct = ft_strnchrl(*regex, '@', jmp);
		if (ft_strchr("=><]", fct[1]))
		{
			ret = default_match(regex, str, len);
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
		*regex += jmp;
		return (0);
	}
	return (-1);
}

static int	wildcard(const char **regex, const char **str, int *len)
{
	while ((*regex)[0] == '*' && (*regex)[1] == '*')
		++*regex;
	if (**regex == '*')
	{
		if (*++*regex == '[')
		{
			if (expanded_wildcard(regex, str, len) != -1)
				return (0);
		}
		while (**str && ft_regex(*regex, *str) == -1)
		{
			++*str;
			++*len;
		}
	}
	else
	{
		if (!**str)
			return (-1);
		++*str;
		++*len;
		++*regex;
	}
	return (0);
}

int	ft_regex(const char *regex, const char *str)
{
	int	pos;
	int	len;

	len = 0;
	while (*regex)
	{
		if (ft_strchr("*?", *regex))
		{
			if (wildcard(&regex, &str, &len) == -1)
				return (-1);
			continue ;
		}
		if ((pos = ft_strpbrkl_pos(regex, "*?")) == -1)
			pos = ft_strlen(regex);
		if (!ft_strnequ(regex, str, pos))
			return (-1);
		len += pos;
		str += pos;
		regex += pos;
	}
	return (*str ? -1 : len);
}
