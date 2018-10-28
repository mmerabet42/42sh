/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_jobs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/16 17:15:39 by jraymond          #+#    #+#             */
/*   Updated: 2018/10/26 19:51:27 by sle-rest         ###   ########.fr       */
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
	t_pids		*a;
	t_inffork	*inf;

	a = ((t_inffork *)elem->content)->pids;
	inf = (t_inffork *)elem->content;
	if (!elem->parent)
		ft_printf("[%d]\t%c %s %s\n",
					inf->x, inf->sign, g_status[inf->status], inf->cmmd);
	else
		ft_printf("\n[%d]\t%c %s %s\n",
					inf->x, inf->sign, g_status[inf->status], inf->cmmd);
	while (a)
	{
		ft_printf("    %-5d %s %s\n", a->pid,
								g_status[((t_inffork *)elem->content)->status],
								a->cmd);
		a = a->next;
	}
}

static	int		error_jobs(char **argv, int error)
{
	if (!error)
		ft_printf_fd(2, "42sh: jobs: %s: no such job\n", argv[1]);
	else if (error == 1)
		ft_printf_fd(2, "42sh: jobs: %s: wrong argument\n", argv[1]);
	else
		ft_printf_fd(2, "42sh: jobs: too many arguments\n");
	return (0);
}

static	int		if_args(int argc, char **argv, int numprocbg)
{
	int		x;
	t_list	*elem;

	x = -1;
	elem = g_shell->bgproc;
	if (argc > 2)
		return (error_jobs(argv, 2));
	while (argv[1][++x] && ft_isdigit(argv[1][x]))
		;
	if (argv[1][x] || numprocbg < 1)
		return (error_jobs(argv, argv[1][x] ? 1 : 0));
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
		return (if_args(argc, argv, ft_atoi(argv[1])));
	while (elem)
	{
		jobs_print(elem);
		elem = elem->next;
	}
	return (0);
}
