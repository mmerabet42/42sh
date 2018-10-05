#include "ft_str.h"
#include "ft_printf.h"

static int	bracket_start(const char *str)
{
	if ((str[0] == '*' || str[0] == '?')
			&& (str[1] == '[' || (str[1] == '!' && str[2] == '[')))
		return (str[1] == '!' ? 3 : 2);
	return (0);
}

static int	bracket_end(const char *str, int l)
{
	if (l && str[0] == '}' && str[1] == ']')
		return (1);
	else if (!l && str[0] == ']')
		return (1);
	return (0);
}

int	regex_bracket(const char *str)
{
	int	i;
	int	j;
	int	l;

	if (!(i = bracket_start(str)))
		return (-1);
	l = 0;
	if (str[i] == '{' && ++i)
		l = 1;
	while (str[i])
	{
		if (bracket_start(&str[i]) || bracket_end(&str[i], l))
		{
			if ((j = regex_bracket(&str[i])) == -1)
				break ;
			i += j;
		}
		else
			++i;
	}
	if (!bracket_end(&str[i], l))
		return (-1);
	return (i + (l ? 2 : 1));
}
