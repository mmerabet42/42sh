/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_jobs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/16 17:15:39 by jraymond          #+#    #+#             */
/*   Updated: 2018/10/16 17:49:17 by jraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_io.h"
#include "shell.h"
#include "ft_types.h"
#include "job_control.h"

static char				*g_status[] = {
	"",
	"Running",
	"Done",
	"Killed",
	"Suspended"
};

static void		jobs_print(t_list *elem)
{
	int		x;
	t_pids	*a;

	a = ((t_inffork *)elem->content)->pids;
	ft_printf("[%d]%c %s %s\n", ((t_inffork *)elem->content)->x,
					((t_inffork *)elem->content)->sign,
					g_status[((t_inffork *)elem->content)->status],
					((t_inffork *)elem->content)->cmmd);
	x = -1;
	while (a)
	{
		ft_printf("  \t  %d %s %s\n", a->pid,
								g_status[((t_inffork *)elem->content)->status],
								a->cmd);
		a = a->next;
	}
}

static	int		error_jobs(char **argv, int error)
{
	if (!error)
		ft_printf("21sh: jobs: %s: no such job\n", argv[1]);
	return (-1);
}

static	int		if_args(char **argv, int numprocbg)
{
	int		x;
	t_list	*elem;

	x = -1;
	elem = g_shell->bgproc;
	while (argv[1][++x] && ft_isdigit(argv[1][x]))
		;
	if (argv[1][x] || numprocbg < 1)
		return (error_jobs(argv, 0));
	while (elem && ((t_inffork *)elem->content)->x != numprocbg)
		elem = elem->next;
	if (!elem)
		return (error_jobs(argv, 0));
	jobs_print(elem);
	return (0);
}

int				builtin_jobs(int argc, char **argv)
{
	t_list	*elem;

	if (!(elem = g_shell->bgproc))
		return (0);
	if (argc > 1)
		return (if_args(argv, ft_atoi(argv[1])));
	while (elem)
	{
		jobs_print(elem);
		elem = elem->next;
	}
	return (0);
}
