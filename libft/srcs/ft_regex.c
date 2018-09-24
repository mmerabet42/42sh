#include "ft_str.h"
#include "ft_printf.h"

static int	wildcard(const char **regex, const char **str, int *len)
{
	int		jmp;

	while ((*regex)[0] == '*' && (*regex)[1] == '*')
		++*regex;
	if (**regex == '*')
	{
		if (*++*regex == '[')
		{
			if ((jmp = ft_strbetweenps_ext((char **)regex, "[?];{?}")) != -1)
			{
				//ft_printf("at: '%d' '%s'\n", jmp, ft_strnchrl(*regex, '@', jmp));
				// *[argument@function_name{=><}{0-9}]
				*regex += jmp;
				return (0);
			}
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
