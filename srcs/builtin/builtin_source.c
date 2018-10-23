/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_source.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/23 20:14:09 by mmerabet          #+#    #+#             */
/*   Updated: 2018/10/23 19:48:56 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_io.h"
#include <fcntl.h>

int			builtin_source(int argc, char **argv)
{
	char	*content;
	int		fd;
	int		res;
	t_ast	*ast;

	if (argc == 1)
		return (!!ft_printf_fd(2, "source: not enough arguments\n"));
	else if ((fd = open(argv[1], O_RDONLY)) == -1
			|| get_next_delimstr(fd, EOF_NEVER_REACH, &content) == -1)
	{
		ft_printf_fd(2, "source: %s: %s\n", ft_strshret(SH_NEXIST), argv[1]);
		close(fd);
		return (127);
	}
	close(fd);
	ast = ft_lexer(content, g_shell->allf->lexerf);
	res = 0;
	if ((fd = ft_astiter(ast, &res, g_shell->allf->iterf)) && fd != SH_EXIT)
		ft_printf_fd(2, "%s: %s: [%s]\n", g_shell->name, ft_strshret(fd),
				content);
	ft_astdel(&ast);
	free(content);
	return (res);
}

static void	incr_arg_builtin_fun(int opt)
{
	if (opt)
	{
		++g_shell->curargs->argv;
		--g_shell->curargs->argc;
		return ;
	}
	--g_shell->curargs->argv;
	++g_shell->curargs->argc;
}

int			builtin_function(int argc, char **argv)
{
	t_list	*it;
	t_func	*func;
	int		res;
	int		ret;
	t_iterf	*itf;

	it = g_shell->funcs;
	if (argc > 1)
	{
		res = 0;
		incr_arg_builtin_fun(1);
		itf = g_shell->allf->iterf;
		if ((func = get_function(argv[1])))
			ret = ft_astiter(func->ast, &res, itf);
		incr_arg_builtin_fun(0);
		if (!func || ret)
			return (!func ? 127 : 1);
		return (0);
	}
	while (it && (func = (t_func *)it->content))
	{
		ft_printf("[%s]\t\t-> {%s}\n", func->name, func->src);
		it = it->next;
	}
	return (0);
}
