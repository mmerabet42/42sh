/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/06 19:27:14 by mmerabet          #+#    #+#             */
/*   Updated: 2018/10/26 16:11:24 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_io.h"
#include "ft_str.h"
#include "ft_mem.h"
#include "shell.h"
#include "ft_types.h"
#include "ft_math.h"
#include "ft_printf_ext.h"
#include "globing.h"
#include "libedit.h"
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>

static t_op			g_ops[] = {
	{"\\:=", OP_BINARY | OP_ASSOCRL},
	{DLM_REDP, OP_BINARY},
	{"not:!", OP_UNARYL},
	{"|", OP_BINARY},
	{"&", OP_BINARY | OP_ASSOCRL},
	{"&&:||", OP_BINARY},
	{",", OP_BINARY | OP_ASSOCRL},
	{"if:while", OP_UNARYL},
	{"then", OP_BINARY | OP_ASSOCRL},
	{"else", OP_BINARY},
	{";:\n", OP_BINARY | OP_ASSOCRL},
};

static t_exp		g_exps[] = {
	{"\\\\*[@=1]", exp_var},
	{"$*[aA0_-zZ9_]:$?", exp_var},
	{"$*[0-9]:$#:$@:$@*[0-9]", exp_arg},
	{"*[$((?));(?);\"*\";'*'@b]", exp_arth},
	{"~", exp_tild},
	{"*[${?};\"*\";'*'@b]", exp_dvar},
	{"*[`?`;$(?);${?};\"*\";'*'@b]", NULL},
	{"*[$\\[*\\];\"*\";'*'@b]", exp_cond},
	{"*[\"*\"@b]:'*':$'*'", NULL},
	{"*[\"'@=1]*[@>0]:$'*[@>0]:\":':$'", NULL},
	{EXP_BRACES, NULL},
	{EXP_SUBSHELL, NULL},
};

static t_expf		g_expf = {
	g_exps, sizeof(g_exps), NULL, 0
};

static t_lexerf		g_lexerf = {
	{
		DLM_WORD, DLM_STOP, DLM_LSTOP, DLM_RSTOP, DLM_HSTOP,
		g_exps, sizeof(g_exps) / sizeof(t_exp), NULL, NULL, NULL
	}, g_ops, sizeof(g_ops) / sizeof(t_op), TK_CMD, TK_OP, NULL
};

static t_astfunc	g_shell_callbacks[] = {
	{EXP_BRACES, shell_lists_cb, NULL, 3},
	{EXP_SUBSHELL, shell_lists_cb, NULL, 3},
	{"*[$\\[*\\];\"*\";'*'@b]", shell_arth_cb, NULL, 3},
	{"*=*", shell_equal, NULL, 3},
	{"", shell_cmd_cb, NULL, 3},
	{"while:if:then", NULL, shell_cond_cb, 0},
	{"else:not:!", shell_else_cb, shell_else_cb, -1},
	{"&&:||", shell_andor_seco_cb, NULL, -1},
	{"&", NULL, shell_bckgrnd_cb, 0},
	{",:;:\n", NULL, shell_andor_seco_cb, 0},
	{"|", NULL, type_pipe, 0},
	{"\\:=", NULL, shell_equal_cb, 0},
	{DLM_REDP, NULL, shell_redir_cb, 0},
};

static t_iterf		g_shell_iterf = {
	g_shell_callbacks, sizeof(g_shell_callbacks) / sizeof(t_astfunc),
	(void *)&g_expf, 0, sizeof(int)
};

t_allf				g_allf;

static char	*init_structs(char *argv0)
{
	char	*name;

	g_expf.data = &g_allf;
	g_lexerf.data = &g_allf;
	g_lexerf.parserf.data = &g_allf;
	g_shell_iterf.data = &g_allf;
	g_expf.data = &g_allf;
	g_allf.parserf = &g_lexerf.parserf;
	g_allf.lexerf = &g_lexerf;
	g_allf.iterf = &g_shell_iterf;
	g_allf.expf = &g_expf;
	name = argv0;
	if ((name = ft_strrchr(argv0, '/')))
		++name;
	return (name);
}

static int	check_script(void)
{
	if (g_shell->bits & (1 << 0))
	{
		g_lexerf.parserf.def_word = DLM_WORD_N;
		g_lexerf.parserf.def_hstop = DLM_HSTOP_N;
	}
	if (g_shell->script)
	{
		++g_shell->curargs->argv;
		--g_shell->curargs->argc;
		ft_interpret(g_shell->script, &g_lexerf, &g_shell_iterf);
		--g_shell->curargs->argv;
		++g_shell->curargs->argc;
		return (1);
	}
	return (0);
}

static int	check_cmd_starter(void)
{
	if (!isatty(1))
		return (1);
	if (g_shell->start_cmd)
		ft_interpret(g_shell->start_cmd, &g_lexerf, &g_shell_iterf);
	return (0);
}

static void	main_execution(char *line)
{
	t_ast	*head;
	int		ret;

	if (*line != 3)
	{
		if (!line[0] && !(g_shell->exitcode = 0) && !check_bgend())
			return ;
		head = ft_lexer(line, &g_lexerf);
		if (check_syntax(head, &g_allf, 0))
			g_shell->exitcode = 1;
		else if ((ret = ft_astiter(head, &g_shell->exitcode, &g_shell_iterf)))
		{
			ft_printshret(ret, line);
			if (ret != SH_EXIT)
				g_shell->exitcode = 1;
		}
		check_bgend();
		ft_astdel(&head);
	}
	else
		g_shell->exitcode = 1;
	if (*line != 3)
		addhistory(line);
}

int			main(int argc, char **argv, char **envp)
{
	char	*line;
	int		cursor;

	shell_begin(init_structs(argv[0]), argc, argv, envp);
	if (check_script() || check_cmd_starter())
		return (shell_end());
	while (g_shell->running)
	{
		printprompt(1);
		ft_getcursor(&cursor, NULL);
		g_shell->kill_builtin = 0;
		if ((line = ft_loop_init(cursor, 0)))
		{
			ft_putchar('\n');
			main_execution(line);
			ft_strdel(&line);
		}
		else
			break ;
	}
	return (shell_end());
}
