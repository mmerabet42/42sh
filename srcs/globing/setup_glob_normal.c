/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_glob_normal.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/02 22:36:51 by sle-rest          #+#    #+#             */
/*   Updated: 2018/09/15 18:14:36 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"
#include "ft_str.h"

int	setup_glob_normal(t_glob **glob, char *splitpath, char *splitacc, int *k)
{
	int	len;

	len = *k;
	while (splitpath[len] && !detect_globbing_at(splitpath, len))
		len++;
	if (!malloc_glob(glob))
		return (0);
	if (!((*glob)->pat = (char *)malloc(sizeof(char) * (len - *k + 1))))
		return (0);
	*((*glob)->pat) = '\0';
	ft_strncat((*glob)->pat, splitpath + *k, len - *k);
	(*glob)->pat[len - *k] = '\0';
	(*glob)->type = normal;
	*k += len - *k;
	(*glob)->slash[G_START] = splitacc[0] == '/' ? 1 : 0;
	(*glob)->slash[G_END] = last_char_splitpaacc_slash(splitacc);
	return (1);
}
