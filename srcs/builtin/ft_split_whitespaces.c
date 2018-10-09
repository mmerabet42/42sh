/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_whitespaces.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/08 16:32:13 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/09 10:16:08 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_str.h"

static int		ft_is_sep(char c)
{
	return (c == '\n' || c == '\t' || c == ' ');
}

static int		ft_count_words(char *str)
{
	int	i;
	int	count;

	i = 0;
	if (ft_is_sep(str[0]) == 0)
	{
		i++;
		count = 1;
	}
	else
		count = 0;
	while (str[i - 1] && str[i])
	{
		if (ft_is_sep(str[i]) == 0 && ft_is_sep(str[i - 1]) == 1)
			count++;
		i++;
	}
	return (count);
}

static int		ft_strlen_sep(char *str, int index)
{
	int	i;
	int	count;

	i = index;
	count = 0;
	while (ft_is_sep(str[i]) == 0 && str[i])
	{
		i++;
		count++;
	}
	return (count);
}

char			**ft_split_whitespaces(char *str)
{
	int		i;
	int		j;
	int		k;
	char	**tab;

	i = 0;
	k = 0;
	if ((tab = malloc(sizeof(char*) * (ft_count_words(str) + 1))) == NULL)
		return (NULL);
	while (ft_is_sep(str[k]) == 1)
		k++;
	while (str[k])
	{
		if (!(tab[i] = malloc(sizeof(char) * (ft_strlen_sep(str, k) + 1))))
			return (NULL);
		j = 0;
		while (ft_is_sep(str[k]) == 0 && str[k])
			tab[i][j++] = str[k++];
		tab[i][j] = '\0';
		while (ft_is_sep(str[k]) == 1 && str[k])
			k++;
		i++;
	}
	tab[i] = NULL;
	return (tab);
}
