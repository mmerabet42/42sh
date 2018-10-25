/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 21:25:42 by mmerabet          #+#    #+#             */
/*   Updated: 2018/10/25 19:23:42 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_str.h"
#include "ft_io.h"
#include "ft_mem.h"
#include <fcntl.h>

static t_exp	g_exps[] = {
	{"\\\\*[$`\"@=1]", exp_var},
	{"$*[aA0_-zZ9_]:$?", exp_var},
	{"$*[0-9]:$@", exp_arg},
	{"*[$((?));(?);\"*\";'*'@b]", exp_arth},
	{"*[${?};\"*\";'*'@b]", exp_dvar},
	{"*[`?`;${?};\"*\";'*'@b]", exp_cmd},
	{"*[$\\[*\\];\"*\";'*'@b]", exp_cond}
};

static t_expf	g_expf = {
	g_exps, sizeof(g_exps), NULL, 0
};

static void	resolve_history_file(char **line)
{
	t_list	*res;
	int		script_fd;

	if (!g_shell->history_file)
		return ;
	res = NULL;
	ft_strexpand(g_shell->history_file, &res, -1, &g_expf);
	g_shell->history_file = res->content;
	res->content = NULL;
	ft_lstdel(&res, content_delfunc);
	if ((script_fd = open(g_shell->history_file,
			O_RDONLY | O_CREAT, 0666)) != -1)
	{
		while (get_next_line(script_fd, line) >= 0)
		{
			addhistory(*line);
			free(*line);
		}
		close(script_fd);
	}
}

static int	shell_exec(int argc, char **argv, char *line)
{
	int		script_fd;

	if (*argv || argc == -1)
	{
		g_shell->history_file = NULL;
		if ((script_fd = (argc == -1 ? 0 : open(*argv, O_RDONLY))) == -1)
			ft_exitf(1, "%s: can't open: %s\n", g_shell->name, *argv);
		ft_memdel((void **)&g_shell->script);
		get_next_delimstr(script_fd, "\01\02\033EOFEOF\n", &g_shell->script);
		if (script_fd)
			close(script_fd);
	}
	else if (g_shell->history_file)
		resolve_history_file(&line);
	return (0);
}

static int	shell_init_bad_opt(t_opt opt)
{
	if (opt.c == '-')
		ft_exitf(1, "%s: bad option: %s\n", g_shell->name, opt.clong);
	else
		ft_exitf(1, "%s: bad option: %c\n", g_shell->name, opt.c);
	return (1);
}

int			shell_init(int argc, char **argv)
{
	t_opt	opt;

	++argv;
	g_shell->history_file = "$HOME/.42sh_history";
	g_shell->start_cmd = "if test -r $HOME/.42shrc then source $HOME/.42shrc";
	while (ft_getopt(&argv, "c.1l.1s.1;log.1", &opt) != OPT_END)
	{
		if (argc && opt.ret == OPT_UNKNOWN)
			return (shell_init_bad_opt(opt));
		else if (opt.c == 'l' || (opt.c == '-' && ft_strequ(opt.clong, "log")))
			g_shell->history_file = (opt.ret == OPT_MISSING
					|| ft_strequ(*opt.ptr, "-") ? NULL : *opt.ptr);
		else if (opt.c == 's')
			g_shell->start_cmd = (opt.ret == OPT_MISSING
					|| ft_strequ(*opt.ptr, "-") ? NULL : *opt.ptr);
		else if (opt.c == 'c')
			g_shell->script = ft_strdup(*opt.ptr);
		else if (opt.c == '-' && ft_strequ(opt.clong, "nsep"))
			g_shell->bits = (1 << 0);
	}
	return (shell_exec((!isatty(0) ? -1 : 1), argv, NULL));
}

void		init_gshell(char **envp, char *name)
{
	g_shell->paths = ft_getpaths(envp);
	g_shell->name = name;
	g_shell->running = 1;
	g_shell->exitcode = 0;
}
