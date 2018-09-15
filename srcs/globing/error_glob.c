/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_glob.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/13 14:10:07 by sle-rest          #+#    #+#             */
/*   Updated: 2018/09/15 17:41:17 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"
#include "ft_str.h"

void	ft_free_tab(char ***tab)
{
	int	i;

	if (!tab && !(*tab))
		return ;
	i = 0;
	while ((*tab)[i])
	{
		ft_strdel(&((*tab)[i]));
		i++;
	}
	free(*tab);
	*tab = NULL;
}

int			error_glob(char *format, ...)
{
	int		i;
	va_list	ap;

	i = 0;
	va_start(ap, format);
	while (format[i])
	{
		if (format[i] == 's')
			ft_strdel(va_arg(ap, char **));
		else if (format[i] == 't')
			ft_free_tab(va_arg(ap, char ***));
		else if (format[i] == 'T')
			free_splitpath(va_arg(ap, char ****));
		else
			free_triple_glob(va_arg(ap, t_glob ****));
		i++;
	}
	return (0);
}
