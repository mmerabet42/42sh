/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_libedit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 11:50:56 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/01 11:33:46 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libedit.h"

int		init_cursor(int prompt, t_cursor *cursor)
{
	struct winsize	ws;

	if (ioctl(0, TIOCGWINSZ, &ws) == -1)
		return (-1);
	cursor->origin = prompt;
	cursor->x = 1;
	cursor->y = 1;
	cursor->ymax = cursor->y;
	cursor->xmax = ws.ws_col;
	return (0);
}

int		ft_new_term(void)
{
	char			*elem_term;
	struct termios	term;

	if (!(elem_term = ft_getenv("TERM", g_shell->envp)))
		return (-1);
	if (tgetent(NULL, elem_term) == -1)
		return (-1);
	if (tcgetattr(0, &term) == -1)
		return (-1);
	term.c_lflag &= ~(ICANON | ECHO | ISIG);
	term.c_iflag &= ~OPOST;
	term.c_cc[VINTR] = -1;
	if (tcsetattr(0, TCSANOW, &term) == -1)
		return (-1);
	return (0);
}

int		ft_clean_term(void)
{
	struct termios term;

	if (tcgetattr(0, &term) == -1)
		return (-1);
	term.c_lflag |= (ICANON | ECHO | ISIG);
	term.c_oflag |= OPOST;
	term.c_cc[VINTR] = 3;
	if (tcsetattr(0, TCSADRAIN, &term) == -1)
		return (-1);
	return (0);
}
