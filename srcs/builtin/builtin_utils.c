/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 13:57:55 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/15 13:58:37 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <sys/ioctl.h>
#include <signal.h>

void	signal_read(int sig)
{
	if (sig == SIGINT)
		ioctl(0, TIOCSTI, "\n");
}
