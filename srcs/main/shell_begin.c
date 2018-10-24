/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_begin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 19:09:16 by mmerabet          #+#    #+#             */
/*   Updated: 2018/10/24 15:40:49 by jraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_str.h"
#include "job_control.h"
#include "ft_mem.h"
#include "ft_io.h"
#include "ft_types.h"
#include "ft_list.h"
#include <unistd.h>
#include <signal.h>
#include <locale.h>
#include <sys/signal.h>
#include <sys/wait.h>

static void		init_sign(void)
{
	setlocale(LC_ALL, "");
	signal(SIGINT, sign_handler);
	signal(SIGWINCH, sign_handler);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGCHLD, sign_child);
	signal(SIGTSTP, SIG_IGN);
}

void			sign_handler(int sign)
{
	char	*tmp;

	if (sign == SIGINT)
	{
		g_shell->kill_builtin = 1;
		kill(-g_shell->curpid, 1);
		if (g_shell->script || g_shell->subshell)
			exit(shell_end());
	}
	else if (sign == SIGWINCH)
	{
		ft_getsize(&g_shell->height, &g_shell->width);
		ft_setenv("LINES", (tmp = ft_itoa(g_shell->height)), &g_shell->envp);
		free(tmp);
		ft_setenv("COLUMNS", (tmp = ft_itoa(g_shell->width)), &g_shell->envp);
		free(tmp);
	}
}

static	void	initenvp(char **envp)
{
	int	i;

	if (!(g_shell->envp =
				(char **)malloc(sizeof(char *) * (ft_tabsize(envp) + 1))))
		ft_exit(EXIT_FAILURE, "Failed to copy 'envp'. Exiting\n");
	i = 0;
	while (*envp)
	{
		if (ft_strmatch(*envp, "USER=*"))
			g_shell->user = ft_strchr(*envp, '=') + 1;
		else if (ft_strmatch(*envp, "HOME=*"))
			g_shell->homepwd = ft_strchr(*envp, '=') + 1;
		if (!(g_shell->envp[i++] = ft_strdup(*envp)))
			ft_exit(EXIT_FAILURE, "Failed to copy 'envp'. Exiting\n");
		++envp;
	}
	g_shell->envp[i] = NULL;
}

int				shell_begin(char *name, int argc, char **argv, char **envp)
{
	static	t_args	args;
	char			*tmp;

	if (!(g_shell = (t_shell *)ft_memalloc(sizeof(t_shell))))
		ft_exit(EXIT_FAILURE, "Failed to begin shell. Exiting\n");
	g_shell->shellpid = getpid();
	init_sign();
	init_gshell(envp, name);
	args.argc = argc;
	args.argv = argv;
	g_shell->curargs = &args;
	g_shell->allf = &g_allf;
	initenvp(envp);
	g_shell->expor = ft_copyenv(g_shell->envp);
	g_shell->localp = ft_copyenv(g_shell->envp);
	ft_bzero(g_shell->pwd, 2048);
	if ((tmp = ft_getenv("PWD", g_shell->envp)))
		ft_strcpy(g_shell->pwd, tmp);
	else
		ft_strcpy(g_shell->pwd, ft_getcwd(NULL, 0));
	update_export("SHLVL",
			(tmp = ft_itoa(ft_atoi(ft_getenv("SHLVL", g_shell->envp)) + 1)));
	free(tmp);
	sign_handler(SIGWINCH);
	return (shell_init(argc, argv));
}
