/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_redir1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/12 20:53:55 by mmerabet          #+#    #+#             */
/*   Updated: 2018/10/17 21:47:21 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_io.h"

void	redir_printerror(t_redir *r, int err, void **op)
{
	(void)op;
	ft_printf_fd(2, "%s: %s", g_shell->name, ft_strshret(err));
	if (err == SH_BADFD)
		ft_printf_fd(2, ": %d\n", r->fdb);
	else if (r->file)
		ft_printf_fd(2, ": %s\n", r->file);
	else
		ft_putchar_fd('\n', 2);
}

int		replace_fd(t_redir *r, int *closed_fd)
{
	if (r->fdb == -1)
	{
		close(r->fda);
		*closed_fd |= (1 << (r->fda));
	}
	else if (r->fdb != -1)
	{
		if (dup2(r->fdb, r->fda) == -1)
			return (SH_BADFD);
	}
	return (0);
}
