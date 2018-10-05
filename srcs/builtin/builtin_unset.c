/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/04 16:31:11 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/05 11:00:41 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	ft_unset(char *name)
{
	if (!name)
		return ;
	ft_unsetenv(name, &g_shell->envp);
	ft_unsetenv(name, &g_shell->expor);
	ft_unsetenv(name, &g_shell->localp);
}

int		builtin_unset(int argc, char **argv)
{
	int	i;

	i = 0;
	while (++i < argc)
		ft_unset(argv[i]);
	return (0);
}
