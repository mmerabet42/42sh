/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selectmode.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/05 20:08:48 by jraymond          #+#    #+#             */
/*   Updated: 2018/09/28 15:38:01 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_printf.h"
#include "ft_io.h"
#include "ft_str.h"

static void	handle_xy_cursor(t_bufferl *bl, int left)
{
	ft_getsize(&bl->h, &bl->w);
	if (left == 1)
	{
		if (bl->cursor-- == 0)
		{
			bl->cursor = 0;
			return ;
		}
		bl->x = (bl->x == 1) ? bl->w : bl->x - 1;
		bl->y = (bl->x == bl->w) ? bl->y - 1 : bl->y;
	}
	else
	{
			if ((int)bl->cursor++ == bl->len - 1)
			{
				bl->cursor--;
				return ;
			}
			bl->x = (bl->x == bl->w) ? 1 : bl->x + 1;
			bl->y = (bl->x == 1) ? bl->y + 1 : bl->y;
	}
}

/*static void	reprint_line2(t_bufferl *bl, int i, int d)
{
	int	y;
	int	tmp;

	y = bl->y;
	tmp = bl->cursor + i;
	ft_getsize(&bl->h, &bl->w);
	ft_printf("\033[%d;%dH\033[s\033[K", bl->y, bl->x);
	while (++y <= bl->h)
		ft_printf("\033[%dF\033[2K", y);
	log_trace("curs: %d line: '%.*s'\n", bl->cursor, i, (bl->line + bl->cursor));
	ft_printf("\033[u%#{white}%{black}%.*s%{0}", i, (bl->line + bl->cursor));
	while (i--)
		d == 'C' ? handle_xy_cursor(bl, 0) : handle_xy_cursor(bl, 1);
	log_trace("curs: %d\n", bl->cursor);
	ft_printf("%s\033[u", (bl->line + tmp));
}
*/
static void	reprint_line3(t_bufferl *bl, t_slctmode *sm)
{
	int	y;
	int	i;

	if ((i = bl->cursor - sm->save_cur ) < 0)
		i *= -1;
	i++;
	ft_getsize(&bl->h, &bl->w);
//	log_trace("curs: %d line: '%.*s'\n", bl->cursor, i, (bl->line + bl->cursor));
	if (sm->save_cur > (int)bl->cursor)
	{
		y = bl->y;
		log_debug("okkkkkkkkkk: %d\n", i);
		ft_printf("\033[%d;%dH\033[s\033[K", bl->y, bl->x);
		while (++y <= bl->h)
			ft_printf("\033[%dE\033[2K", y);
		ft_printf("\033[u%#{white}%{black}%.*s%{0}", i, (bl->line + bl->cursor));
		ft_printf("%s\033[u", (bl->line + sm->save_cur + 1));
	}
	else
	{
		y = sm->save_y;
		log_debug("oooooooooook: %d\n", i);
		ft_printf("\033[%d;%dH\033[s\033[K", sm->save_y, sm->save_x);
		while (++y <= bl->h)
			ft_printf("\033[%dE\033[2K", y);
		ft_printf("\033[u%#{white}%{black}%.*s%{0}", i, (bl->line + sm->save_cur));
		ft_printf("%s\033[%d;%dH", (bl->line + bl->cursor + 1), sm->save_y, sm->save_x);
	}
//	log_trace("curs: %d\n", bl->cursor);
}

static void	current(t_bufferl *bl)
{
	log_trace("CURRENT: x: %d y: %d cursor: %d\n", bl->x, bl->y, bl->cursor);
	log_trace("CURRENT: w: %d h: %d\n", bl->w, bl->h);
}

/*int			selectmode(t_bufferl *bl, int c)
{
	static char	buff[500];
	int			i;
	int			d;

	d = 0;
	i = 1;
	*buff = '\0';
	if (!bl->cursor)
		return (0);
	if (c == 'R')
	{
		if (!bl->line[bl->cursor])
		{
			log_debug("start selectmode: '%c'\n", bl->line[bl->cursor - 1]);
			ft_printf("\033[D");
			handle_xy_cursor(bl, 1);
		}
		else
			log_debug("start selectmode: '%c'\n", bl->line[bl->cursor]);
		while (1)
		{
			if ((d == 27 || (d = ft_getch()) == 27) &&
					(d = ft_getch()) == 91 && (d = ft_getch()))
			{
				if (d == 'C')
				{
					log_trace("BFORE: x: %d y:%d\n", bl->x, bl->y);
					handle_xy_cursor(bl, 0);
					log_trace("AFTER: x: %d y:%d\n", bl->x, bl->y);
					i++;
					reprint_line2(bl, i, d);
				}
				else if (d == 'D')
				{
					log_trace("BFORE: x: %d y:%d\n", bl->x, bl->y);
					handle_xy_cursor(bl, 1);
					log_trace("AFTER: x: %d y:%d\n", bl->x, bl->y);
					i++;
					reprint_line2(bl, i, d);
				}
				else if (d == '1' && (d = ft_getch()) == ';' && (d = ft_getch()) == '2'
							&& ((d = ft_getch()) == 'R' || d == 'S'))
				{
					log_debug("enddddddd selectmode\n");
					break;
				}
				current();
			}
		}
	}
	return (0);
}*/

static void	copy(char *buff, t_bufferl *bl, t_slctmode *sm)
{
	if (sm->save_cur > (int)bl->cursor)
		ft_strncpy(buff, (bl->line + bl->cursor), (sm->save_cur - bl->cursor + 1));
	else
		ft_strncpy(buff, (bl->line + sm->save_cur), (bl->cursor - sm->save_cur + 1));
	log_trace("COPY: '%s'\n", buff);
}

static void	initsm(t_slctmode *sm, t_bufferl *bl)
{
	sm->save_x = bl->x;
	sm->save_y = bl->y;
	sm->save_cur = bl->cursor;
}

int			selectmode(t_bufferl *bl, int c)
{
	t_slctmode	sm;
	static char	buff[500];
	int			i;

	if (!bl->cursor)
		return (0);
	if (c == 'R')
	{
		c = 0;
		if (!bl->line[bl->cursor])
		{
			ft_printf("\033[D");
			handle_xy_cursor(bl, 1);
		}
		log_debug("start selectmode: '%c'\n", bl->line[bl->cursor]);
		initsm(&sm, bl);
		while (1)
		{
			if ((c == 27 || (c = ft_getch()) == 27) &&
					(c = ft_getch()) == 91 && (c = ft_getch()))
			{
				if (c == 'C')
				{
					ft_printf("\033[s\033[%d;%dH%s\033[u", bl->ys, bl->xs, bl->line);
					log_trace("BFORE: x: %d y:%d\n", bl->x, bl->y);
					handle_xy_cursor(bl, 0);
					log_trace("AFTER: x: %d y:%d\n", bl->x, bl->y);
					reprint_line3(bl, &sm);
				}
				else if (c == 'D')
				{
					log_trace("BFORE: x: %d y:%d\n", bl->x, bl->y);
					handle_xy_cursor(bl, 1);
					log_trace("AFTER: x: %d y:%d\n", bl->x, bl->y);
					reprint_line3(bl, &sm);
				}
				else if (c == '1' && (c = ft_getch()) == ';' && (c = ft_getch()) == '2'
							&& ((c = ft_getch()) == 'R' || c == 'S'))
				{
					log_debug("enddddddd selectmode\n");
					if (c == 'S')
						copy(buff, bl, &sm);
					break;
				}
				current(bl);
			}
		}
		ft_printf("\033[s\033[%d;%dH%s\033[u", bl->ys, bl->xs, bl->line);
	}
	else if (c == 'S')
	{
		i = -1;
		while (buff[++i])
			add_char(bl, (int)buff[i]);
//		ft_strinsert((bl->line + bl->cursor), buff, 0, ft_strlen(buff));
//		ft_printf("\033[s\033[%d;%dH%s\033[u", bl->ys, bl->xs, bl->line);
//		bl->len = ft_strlen(bl->line);
	}
	return (0);
}
