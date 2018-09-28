/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readraw.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/29 17:13:32 by mmerabet          #+#    #+#             */
/*   Updated: 2018/09/28 15:37:55 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_io.h"
#include "ft_str.h"
#include "ft_mem.h"

void		escape_char(t_bufferl *bl, int c)
{
	if ((c = (!c && ft_getch() ? ft_getch() : c)) == 'D' && bl->cursor != 0)
	{
		--bl->x;
		if (--bl->cursor && bl->x <= 0)
		{
			ft_getsize(NULL, &bl->x);
			ft_printf("\033[A\033[%dC", bl->x, --bl->y);
		}
		else
			ft_putstr("\033[D");
	}
	else if (c == 'C' && (int)bl->cursor < bl->len)
	{
		ft_getsize(NULL, &bl->w);
		if (++bl->cursor && ++bl->x > bl->w)
			ft_printf("\n", (bl->x = 1), ++bl->y);
		else
			ft_putstr("\033[C");
	}
	else if ((c == 'A' || c == 'B') && g_shell->history)
		move_history(bl, c);
	else if (c == 'H' || c == 'F' || c == '5' || c == '6')
		move_end(bl, c);
	else if (c == '1' && ft_getch() == ';' && ft_getch() == '2')
		move_word(bl, ft_getch());
}

static int	init_bl(t_bufferl *bl, char *line, size_t size)
{
	int	ny;

	if (!line)
	{
		ft_getsize(&bl->h, &bl->w);
		ny = ((bl->len + bl->xs) - (bl->cursor + bl->xs) + bl->x) / bl->w;
		ft_putstr("\033[?25h");
		while (ny--)
			ft_putchar('\n');
		return (0);
	}
	ft_bzero(bl, sizeof(t_bufferl));
	bl->it = g_shell->history;
	bl->line = line;
	bl->size = size;
	ft_getcursor(&bl->x, &bl->y);
	bl->ys = bl->y;
	bl->xs = bl->x;
	return (0);
}

int			ft_readraw(char *line, size_t size)
{
	int			c;
	t_bufferl	bl;

	init_bl(&bl, line, size);
	ft_makeraw(1);
	while ((c = ft_getch()))
	{
		if ((c == 3 || c == 4 || c == 13) && !init_bl(&bl, NULL, 0))
			break ;
		if (c == 127 && !bl.sm)
			delete_char(&bl);
		else if (c == 27)
			escape_char(&bl, 0);
		else if (!bl.sm)
			add_char(&bl, c);
		if (bl.sm)
			reprint_sm(&bl);
	}
	ft_makeraw(0);
	free(bl.save);
	return (c);
}
