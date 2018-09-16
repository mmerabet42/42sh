/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_match_glob.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/01 22:54:18 by sle-rest          #+#    #+#             */
/*   Updated: 2018/09/15 18:10:28 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"

static int	(*g_check_match[4])(t_glob **glob, char *name, int *i) = {
	&check_match_normal,
	&check_match_stars,
	&check_match_question,
	&check_match_crochet
};

int	is_match_stars(t_glob **glob, char *name, int *i)
{
	int	type;

	while (glob && *glob && name[*i])
	{
		type = (*glob)->type;
		type = type > 2 ? 3 : type;
		if (!g_check_match[type](glob, name, i))
			return (0);
		if (type == stars)
			return (1);
		*glob = (*glob)->next;
	}
	while (*glob && (*glob)->type == stars)
		*glob = (*glob)->next;
	if (!(*glob) && !name[*i])
		return (1);
	return (0);
}

int	is_match_glob(t_glob **glob, struct dirent *dp)
{
	int		i;
	int		type;
	t_glob	*cglob;

	i = 0;
	cglob = *glob;
	while (cglob && dp->d_name[i])
	{
		type = cglob->type;
		type = type > 2 ? 3 : type;
		if (!g_check_match[type](&cglob, dp->d_name, &i))
			return (0);
		if (type == stars)
			return (1);
		cglob = cglob->next;
	}
	while (cglob && cglob->type == stars)
		cglob = cglob->next;
	if (!cglob && !dp->d_name[i])
		return (1);
	return (0);
}
