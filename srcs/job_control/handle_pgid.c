/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pgid.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/17 13:22:01 by jraymond          #+#    #+#             */
/*   Updated: 2018/09/18 17:10:33 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** set pgid of curprocess at curprocess, and give at curprocess law
** of entry 0;
*/

#include "shell.h"
#include <sys/signal.h>

void	handle_pgid()
{
	signal(SIGINT, SIG_DFL);
	setpgid(0, 0);
	tcsetpgrp(0, getpgrp());
}
