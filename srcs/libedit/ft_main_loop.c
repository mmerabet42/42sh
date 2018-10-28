/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_main_loop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/23 20:02:13 by sle-rest          #+#    #+#             */
/*   Updated: 2018/10/27 10:22:27 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libedit.h"

t_cmdedit	*ft_main_loop(t_cmdedit *cmd, t_cursor *cursor)
{
	char	buf[8];
	int		ret;

	while (1)
	{
		ft_bzero(buf, 7);
		if ((ret = read(0, buf, 7)) == -1)
			break ;
		buf[ret] = 0;
		if (buf[0] == '\n')
			break ;
		if (!(cmd = ft_parser_edit(buf, cmd, cursor)))
			break ;
	}
	while (cmd && cmd->next)
		cmd = move_cursor(cmd, cursor, 'C');
	return (cmd);
}
