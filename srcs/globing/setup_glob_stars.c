/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_glob_stars.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/02 22:37:13 by sle-rest          #+#    #+#             */
/*   Updated: 2018/09/15 18:13:59 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"

int	setup_glob_stars(t_glob **glob, char *splitpath, char *splitacc, int *k)
{
	(void)splitpath;
	if (!malloc_glob(glob))
		return (0);
	(*glob)->type = stars;
	(*k)++;
	(*glob)->slash[G_START] = splitacc[0] == '/' ? 1 : 0;
	(*glob)->slash[G_END] = last_char_splitpaacc_slash(splitacc);
	return (1);
}
