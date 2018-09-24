/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_libedit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 11:50:56 by gdufay            #+#    #+#             */
/*   Updated: 2018/09/24 16:42:47 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libedit.h"

int		init_cursor(char *prompt, t_cursor *cursor)
{
	struct winsize	ws;

	if (ioctl(0, TIOCGWINSZ, &ws) == -1)
		return (-1);
	cursor->origin = ft_strlen(prompt);
	cursor->x = cursor->origin;
	cursor->y = 0;
	cursor->ymax = cursor->y;
	cursor->xmax = ws.ws_col;
	return (0);
}

int		ft_new_term(t_env *lstenv)
{
	t_env			*elem_term;
	struct termios	term;

	if (!(elem_term = get_elem_lstenv("TERM", lstenv)))
		return (-1);
	if (tgetent(NULL, elem_term->value) == -1)
		return (-1);
	if (tcgetattr(0, &term) == -1)
		return (-1);
	term.c_lflag &= ~(ICANON | ECHO);
	term.c_oflag &= ~OPOST;
	term.c_cc[VINTR] = -1;
	if (tcsetattr(0, TCSADRAIN, &term) == -1)
		return (-1);
	return (0);
}

int		ft_clean_term(void)
{
	struct termios term;

	if (tcgetattr(0, &term) == -1)
		return (-1);
	term.c_lflag |= ICANON;
	term.c_lflag |= ECHO;
	term.c_oflag |= OPOST;
	term.c_cc[VINTR] = 3;
	if (tcsetattr(0, TCSADRAIN, &term) == -1)
		return (-1);
	return (0);
}
