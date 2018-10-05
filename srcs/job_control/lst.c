#include "shell.h"
#include "ft_mem.h"
#include "job_control.h"

static t_pids	*creat_elem(pid_t pid)
{
	t_pids	*pids;

	if (!(pids = (t_pids *)ft_memalloc(sizeof(t_pids))))
		return (NULL);
	pids->pid = pid;
	return (pids);
}

int				creatpushelem(t_pids **head, pid_t pid)
{
	t_pids	*pids;
	t_pids	*elem;

	if (!(pids = creat_elem(pid)))
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
