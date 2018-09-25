/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 13:22:47 by gdufay            #+#    #+#             */
/*   Updated: 2018/07/17 10:55:27 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libedit.h"

void	exec_t(char *s)
{
	tputs(tgetstr(s, NULL), 0, ft_putc);
}

void	mv_bnl(void)
{
	exec_t("do");
	exec_t("cr");
}

void	mv_until_end(int len)
{
	while (--len > 0)
		exec_t("nd");
}

void	write_char(char c)
{
	if (c < 0)
		return ;
	exec_t("im");
	exec_t("ic");
	ft_putc(c);
	exec_t("ei");
}
