/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_glob_question.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/02 22:38:35 by sle-rest          #+#    #+#             */
/*   Updated: 2018/09/15 18:14:24 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"

int	setup_glob_question(t_glob **glob, char *splitpath, char *splitacc, int *k)
{
	(void)splitpath;
	if (!malloc_glob(glob))
		return (0);
	(*glob)->type = question;
	(*k)++;
	(*glob)->slash[G_START] = splitacc[0] == '/' ? 1 : 0;
	(*glob)->slash[G_END] = last_char_splitpaacc_slash(splitacc);
	return (1);
}
