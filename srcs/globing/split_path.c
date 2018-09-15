/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/28 00:28:44 by sle-rest          #+#    #+#             */
/*   Updated: 2018/09/15 18:17:28 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"
#include "ft_str.h"

static int	get_nb_path(char *splitacc)
{
	int	i;
	int	len;

	i = 0;
	len = 1;
	while (splitacc[i] == '/')
		i++;
	while (splitacc[i])
	{
		len = splitacc[i] == '/' ? len + 1 : len;
		if (splitacc[i] == '/')
		{
			while (splitacc[i] == '/')
				i++;
		}
		else
			i++;
	}
	return (len);
}

static int	get_len_between_path(char *splitacc, int j)
{
	int	len;

	len = 0;
	while (splitacc[j])
	{
		if (splitacc[j] == '/')
			break ;
		j++;
		len++;
	}
	return (len);
}

static int	loop_split_path(char ***splitpath, char **splitacc, int i)
{
	int		len;
	int		j;
	int		x;

	len = get_nb_path(splitacc[i]);
	if (!(splitpath[i] = (char **)malloc(sizeof(char *) * (len + 1))))
		return (0);
	j = 0;
	x = 0;
	while (splitacc[i][j])
	{
		while (splitacc[i][j] == '/')
			j++;
		len = get_len_between_path(splitacc[i], j);
		if (!(splitpath[i][x] = (char *)malloc(sizeof(char) * (len + 1))))
			return (0);
		ft_strncpy(splitpath[i][x], splitacc[i] + j, len);
		splitpath[i][x][len] = '\0';
		j += len;
		while (splitacc[i][j] == '/')
			j++;
		x++;
	}
	splitpath[i][x] = NULL;
	return (1);
}

char		***split_path(char **splitacc)
{
	char	***splitpath;
	int		len;
	int		i;

	len = (int)ft_tabsize(splitacc);
	splitpath = NULL;
	if (!(splitpath = (char ***)malloc(sizeof(char **) * (len + 1))))
		return (NULL);
	i = 0;
	while (splitacc[i])
	{
		if (!loop_split_path(splitpath, splitacc, i))
			return (0);
		i++;
	}
	splitpath[i] = NULL;
	return (splitpath);
}
