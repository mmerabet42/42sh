/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 11:02:45 by jraymond          #+#    #+#             */
/*   Updated: 2018/10/19 16:15:25 by jraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_mem.h"
#include "ft_str.h"
#include "job_control.h"

static char		*cmdpipe(t_list *elem)
{
	t_ast	*a;
	char	*cmd;
	int		all_size;
	int		x;

	all_size = 3;
	x = -1;
	a = (t_ast *)elem->content;
	while (a->left)
		a = a->left;
	while (a->args->argv[++x])
		all_size += (ft_strlen(a->args->argv[x]) + 1);
	if (!(cmd = (char *)ft_memalloc(sizeof(char) * all_size)))
		return (NULL);
	x = -1;
	while (a->args->argv[++x])
	{
		ft_memcpy(&cmd[ft_strlen(cmd)],
					a->args->argv[x], ft_strlen(a->args->argv[x]));
		if (a->args->argv[x + 1])
			cmd[ft_strlen(cmd)] = ' ';
	}
	return (cmd);
}

static t_pids	*creat_elem(pid_t pid, t_list *elem, int bquote)
{
	t_pids	*pids;

	if (!(pids = (t_pids *)ft_memalloc(sizeof(t_pids))))
		return (NULL);
	pids->pid = pid;
	if (!bquote && !(pids->cmd = cmdpipe(elem)))
		return (NULL);
	return (pids);
}

int				creatpushelem(t_pids **head, pid_t pid, t_list *ast, int bquote)
{
	t_pids	*pids;
	t_pids	*elem;

	if (!(pids = creat_elem(pid, ast, bquote)))
		return (SH_MALLOC);
	if (!*head)
		*head = pids;
	else
	{
		elem = *head;
		while (elem->next)
			elem = elem->next;
		elem->next = pids;
		pids->parent = elem;
	}
	return (0);
}

void			extractpids(t_pids **head, pid_t pid)
{
	t_pids	*elem;

	elem = *head;
	while (elem && elem->pid != pid)
		elem = elem->next;
	if (elem && elem == *head)
	{
		if ((*head = elem->next))
			elem->next->parent = NULL;
	}
	else if (elem)
	{
		elem->parent->next = elem->next;
		elem->next->parent = elem->parent;
	}
	if (elem)
		ft_memdel((void **)&elem);
}
