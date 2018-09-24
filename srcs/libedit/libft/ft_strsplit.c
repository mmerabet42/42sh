/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 12:39:14 by gdufay            #+#    #+#             */
/*   Updated: 2017/11/15 10:42:10 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		ft_count_words(const char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	if (str[0] != c)
	{
		i++;
		count = 1;
	}
	else
		count = 0;
	while (str[i])
	{
		if (str[i] != c && str[i - 1] == c)
			count++;
		i++;
	}
	return (count);
}

static int		ft_strlen_sep(const char *str, int index, char c)
{
	int	i;
	int	count;

	i = index;
	count = 0;
	while (str[i] != c && str[i])
	{
		i++;
		count++;
	}
	return (count);
}

char			**ft_strsplit(const char *str, char c)
{
	int		i;
	int		j;
	int		k;
	char	**tab;

	i = 0;
	k = 0;
	if (!str || !(tab = malloc(sizeof(char*) * (ft_count_words(str, c) + 1))))
		return (NULL);
	while (str[k] == c)
		k++;
	while (str[k])
	{
		if (!(tab[i] = malloc(sizeof(char) * (ft_strlen_sep(str, k, c) + 1))))
			return (NULL);
		j = 0;
		while (str[k] != c && str[k])
			tab[i][j++] = str[k++];
		tab[i][j] = '\0';
		while (str[k] == c && str[k])
			k++;
		i++;
	}
	tab[i] = 0;
	return (tab);
}
