/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_glob_crochet.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/02 22:37:35 by sle-rest          #+#    #+#             */
/*   Updated: 2018/09/15 18:14:16 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"

int	setup_glob_crochet_2(t_glob **glob, char *splitpath, char *splitacc, int *k)
{
	int	i;
	int	c_b;
	int	c_e;

	c_b = splitpath[++(*k)];
	*k = *k + 2;
	c_e = splitpath[*k];
	*k = *k + 2;
	if (!malloc_glob(glob))
		return (0);
	if (!((*glob)->pat = (char *)malloc(sizeof(char) * (c_e - c_b + 2))))
		return (0);
	i = 0;
	while (c_b <= c_e)
	{
		(*glob)->pat[i] = c_b;
		c_b++;
		i++;
	}
	(*glob)->pat[i] = '\0';
	(*glob)->type = crochet;
	(*glob)->slash[G_START] = splitacc[0] == '/' ? 1 : 0;
	(*glob)->slash[G_END] = last_char_splitpaacc_slash(splitacc);
	return (1);
}

int	setup_glob_crochet(t_glob **glob, char *splitpath, char *splitacc, int *k)
{
	if (detect_globbing_at(splitpath, *k) == 5)
		return (setup_glob_crochet_2(glob, splitpath, splitacc, k));
	if (!malloc_glob(glob))
		return (0);
	if (!((*glob)->pat = (char *)malloc(sizeof(char) * 2)))
		return (0);
	(*glob)->pat[0] = splitpath[*k + 1];
	(*glob)->pat[1] = '\0';
	(*glob)->type = crochet;
	*k = *k + 3;
	(*glob)->slash[G_START] = splitacc[0] == '/' ? 1 : 0;
	(*glob)->slash[G_END] = last_char_splitpaacc_slash(splitacc);
	return (1);
}
