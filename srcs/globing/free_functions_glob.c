/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions_glob.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/09 16:29:02 by sle-rest          #+#    #+#             */
/*   Updated: 2018/09/15 18:05:54 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_str.h"
#include "globing.h"

void		free_acc(t_acc **acc)
{
	t_acc	*tmp;
	int		i;

	while (acc && *acc)
	{
		if ((*acc)->type == normal)
			ft_strdel(&((*acc)->str));
		else
		{
			i = -1;
			while ((*acc)->split[++i])
				ft_strdel(&((*acc)->split[i]));
			free((*acc)->split);
		}
		tmp = (*acc)->next;
		free(*acc);
		*acc = tmp;
	}
}

void		free_splitpath(char ****splitpath)
{
	int	i;
	int	j;

	i = -1;
	while (splitpath && *splitpath && (*splitpath)[++i])
	{
		j = -1;
		while (**splitpath && (*splitpath)[i][++j])
			ft_strdel(&((*splitpath)[i][j]));
		free((*splitpath)[i]);
	}
	free(*splitpath);
	*splitpath = NULL;
}

static void	free_glob(t_glob **glob)
{
	t_glob	*tmp;

	while (glob && *glob)
	{
		tmp = (*glob)->next;
		ft_strdel(&((*glob)->pat));
		free(*glob);
		*glob = tmp;
	}
	*glob = NULL;
}

void		free_triple_glob(t_glob ****glob)
{
	int	i;
	int	j;

	i = -1;
	while ((*glob)[++i])
	{
		j = -1;
		while ((*glob)[i][++j])
		{
			free_glob(&((*glob)[i][j]));
			(*glob)[i][j] = NULL;
		}
		free((*glob)[i]);
		(*glob)[i] = NULL;
	}
	free(*glob);
}
