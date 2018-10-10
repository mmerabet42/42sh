/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pgid.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 13:22:01 by jraymond          #+#    #+#             */
/*   Updated: 2018/10/10 14:14:48 by jraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** set pgid of curprocess at curprocess, and give at curprocess law
** of entry 0;
*/

#include "shell.h"
#include <sys/signal.h>

void	resetsign(void)
{
	int	x;

	x = -1;
	while (++x < 33)
		signal(x, SIG_DFL);
}

void	handle_pgid(void)
{
	setpgid(0, 0);
	tcsetpgrp(0, getpgrp());
	resetsign();
}
