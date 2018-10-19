/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_matches.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 17:43:10 by mmerabet          #+#    #+#             */
/*   Updated: 2018/10/19 17:47:34 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_str.h"

int	get_matches(t_regex_info *rgxi)
{
	t_list			*head;
	t_regex_match	matchs[2];
	int				zero;
	int				i;

	rgxi->pos = &(matchs[0].pos);
	rgxi->id = &(matchs[0].id);
	matchs[0].str_begin = rgxi->str;
	matchs[1].str_begin = rgxi->str;
	head = NULL;
	i = 0;
	matchs[1].pos = 0;
	zero = 0;
	while ((matchs[0].len = regex_pos(rgxi)) != -1)
	{
		if ((rgxi->flags & RGX_UGLOBAL) && matchs[0].pos)
		{
			matchs[1].len = matchs[0].pos + (zero ? 1 : 0);
			matchs[1].str = matchs[1].str_begin + matchs[1].pos;
			matchs[1].id = -1;
			ft_lstpush_p(&head, ft_lstnew(&matchs[1], sizeof(t_regex_match)));
		}
		matchs[0].pos += matchs[1].pos;
		if (rgxi->flags & RGX_GLOBAL)
		{
			matchs[0].str = matchs[0].str_begin + matchs[0].pos;
			ft_lstpush_p(&head, ft_lstnew(&matchs[0], sizeof(t_regex_match)));
			matchs[0].id = 0;
		}
		++i;
		zero = (!matchs[0].len ? 1 : 0);
		matchs[1].pos = matchs[0].pos + (!matchs[0].len ? 1 : matchs[0].len);
		if (!*(matchs[0].str_begin + matchs[0].pos)
				|| !*(rgxi->str = matchs[0].str_begin + matchs[1].pos))
			break ;
		rgxi->regex = rgxi->rgx_begin;
		rgxi->len = 0;
	}
	if ((rgxi->flags & RGX_UGLOBAL) && matchs[0].len == -1)
	{
		matchs[1].len = matchs[0].pos;
		matchs[1].str = matchs[0].str_begin + matchs[1].pos;
		matchs[1].id = -1;
		ft_lstpush_p(&head, ft_lstnew(&matchs[1], sizeof(t_regex_match)));
	}
	*rgxi->matches = head;
	return (i);
}
