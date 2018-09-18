/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_get_glob.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/28 22:40:43 by sle-rest          #+#    #+#             */
/*   Updated: 2018/09/18 16:41:51 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"
#include "ft_str.h"

static int	(*g_setup_glob[4])(t_glob **glob, char *splitpath,
								char *splitacc, int *k) = {
	&setup_glob_normal,
	&setup_glob_stars,
	&setup_glob_question,
	&setup_glob_crochet
};

int			last_char_splitpaacc_slash(char *splitacc)
{
	int		i;

	i = 0;
	while (splitacc[i] && splitacc[i + 1])
		i++;
	return (splitacc[i] == '/' ? 1 : 0);
}

static int	inception_loop_loop_get_glob(t_glob **glob, char *splitpath,
											char *splitacc, int *k)
{
	int	detect;

	detect = detect_globbing_at(splitpath, *k);
	detect = detect == 4 || detect == 5 ? 3 : detect;
	if (!g_setup_glob[detect](glob, splitpath, splitacc, k))
	{
		while (*glob && (*glob)->prev)
			*glob = (*glob)->prev;
		return (0);
	}
	return (1);
}

static int	inception_loop_get_glob(char ***splitpath, t_glob ***glob,
										char **splitacc, int i)
{
	int	j;
	int	k;

	j = -1;
	while (splitpath[i][++j])
	{
		k = splitpath[i][j][0] == '/' ? 1 : 0;
		glob[i][j] = NULL;
		while (splitpath[i][j][k])
		{
			if (!inception_loop_loop_get_glob(&(glob[i][j]), splitpath[i][j],
					splitacc[i], &k))
				return (0);
		}
		while (glob[i][j] && glob[i][j]->prev)
			glob[i][j] = glob[i][j]->prev;
	}
	glob[i][j] = NULL;
	return (1);
}

int			loop_get_glob(char ***splitpath, t_glob ***glob, char **splitacc,
							int i)
{
	int	len;

	len = (int)ft_tabsize(splitpath[i]);
	if (!(glob[i] = (t_glob **)malloc(sizeof(t_glob *) * (len + 1))))
		return (0);
	if (!inception_loop_get_glob(splitpath, glob, splitacc, i))
		return (0);
	return (1);
}
