/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_bgend.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/19 16:04:49 by jraymond          #+#    #+#             */
/*   Updated: 2018/10/20 17:52:44 by ouralgan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_mem.h"
#include "ft_io.h"

static char				*g_status[] = {
	"",
	"Running",
	"Done",
	"Killed",
	"Suspended"
};

void					print_cmd_args2(char *tab)
{
	if (!tab)
		return ;
	ft_printf("%s\n", tab);
}

static t_list			*delete_info(t_list *elem)
{
	int		i;
	t_list	*save;

	i = -1;
	save = elem->next;
	ft_lstextract(&g_shell->bgproc, elem);
	ft_lstdelone(&elem, del);
	if (i == 0)
		return (g_shell->bgproc);
	else
		return (save);
}

static t_list			*check_bgend_bis(t_inffork *struc, t_list *elem)
{
	ft_printf("[%d]\t%c %s\t%s\n", struc->x, struc->sign,
				g_status[struc->status], struc->cmmd);
	struc->modif &= (0 << 0);
	return (elem->next);
}

int						check_bgend(void)
{
	t_list		*elem;
	t_inffork	*struc;

	elem = g_shell->bgproc;
	while (elem)
	{
		struc = elem->content;
		if (struc->status == BG_END || struc->status == BG_KILL)
		{
			if (!struc->cmmd)
				ft_printf("[%d]\t%c %s\t%s\n", struc->x, struc->sign,
							g_status[struc->status], struc->cmmd);
			else
				ft_printf("[%d]\t%c %s\t%s\n", struc->x, struc->sign,
							g_status[struc->status], struc->cmmd);
			handle_bgsign(elem, 1);
			elem = delete_info(elem);
		}
		else if (struc->modif & (1 << 0))
			elem = check_bgend_bis(struc, elem);
		else
			elem = elem->next;
	}
	return (0);
}
