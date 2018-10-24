/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/12 12:28:55 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/24 18:36:42 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_io.h"
#include "ft_mem.h"
#include "ft_printf.h"
#include "ft_str.h"

static void	print_pwd(int h)
{
	int		len;

	if (h)
	{
		len = (int)ft_strlen(g_shell->homepwd);
		if (ft_strnequ(g_shell->pwd, g_shell->homepwd, len))
		{
			ft_putchar('~');
			ft_putstr(g_shell->pwd + len);
			return ;
		}
	}
	ft_putstr(g_shell->pwd);
}

int			builtin_pwd(int argc, char **argv)
{
	t_opt	opt;

	argc = 0;
	ft_bzero(&opt, sizeof(t_opt));
	++argv;
	while (ft_getopt(&argv, "LPnh", &opt) != OPT_END)
	{
		if (opt.c == 'L')
			argc &= (~(1 << 0));
		else if (opt.c == 'P')
			argc |= (1 << 0);
		else if (opt.c == 'n')
			argc |= (1 << 1);
		else if (opt.c == 'h')
			argc |= (1 << 2);
		else if (ft_printf_fd(2, "pwd: bad option: %c\n", opt.c))
			return (1);
	}
	if (argc & (1 << 0))
		ft_printf("%s", ft_getcwd(NULL, 0));
	else
		print_pwd((argc & (1 << 2)));
	if (!(argc & (1 << 1)))
		ft_printf("\n");
	return (0);
}
