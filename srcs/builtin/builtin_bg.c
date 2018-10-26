/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_bg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/07 16:47:10 by jraymond          #+#    #+#             */
/*   Updated: 2018/10/26 17:25:00 by jraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_io.h"
#include "ft_types.h"
#include <sys/types.h>
#include <signal.h>

static	t_list		*error_bg(char **argv, int i)
{
	if (!i)
		ft_printf_fd(2, "42sh: bg: %s: no such job\n", argv[1]);
	else
		ft_putstr_fd("42sh: bg: no current job\n", 2);
	return (NULL);
}

static	t_list		*check_args(int argc, char **argv, int numprocbg)
{
	int		x;
	t_list	*elem;

	x = -1;
	if (!(elem = g_shell->bgproc))
		return (error_bg(argv, 1));
	if (argc > 1)
	{
		while (argv[1][++x] && ft_isdigit(argv[1][x]))
			;
		if (argv[1][x] || numprocbg < 1)
			return (error_bg(argv, 0));
		while (elem && ((t_inffork *)elem->content)->x != numprocbg)
			elem = elem->next;
		handle_bgsign(elem, 0);
	}
	else
		while (elem && ((t_inffork *)elem->content)->sign != '+')
			elem = elem->next;
	if (elem && (((t_inffork *)elem->content)->status != BG_KILL ||
				((t_inffork *)elem->content)->status != BG_END))
		return (elem);
	return (error_bg(argv, 0));
}

int					builtin_bg(int argc, char **argv)
{
	t_list	*elem;
	pid_t	pid;
	int		ret;

	ret = ft_atoi(argv[1]);
	if (!(elem = check_args(argc, argv, ret)))
		return (1);
	else
	{
		if (((t_inffork *)elem->content)->status != BG_STOP)
			ft_printf_fd(2, "42sh: bg: job %d already in background\n", ret);
		else
		{
			pid = ((t_inffork *)elem->content)->pid;
			pid = pid == -1 ? ((t_inffork *)elem->content)->pids->pid : pid;
			ret = ((t_inffork *)elem->content)->pid == -1 ? 1 : 0;
			handle_bgstat(pid, BG_RUN, ret);
			((t_inffork *)elem->content)->modif |= (1 << 0);
			if (((t_inffork *)elem->content)->pid == -1)
				kill(-pid, SIGCONT);
			else
				kill(pid, SIGCONT);
		}
	}
	return (0);
}
