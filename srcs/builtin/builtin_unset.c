/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/04 16:31:11 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/04 16:43:07 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int					builtin_unset(int argc, char **argv)
{
	int	i;

	i = 0;
	while (++i < argc)
	{
		ft_unsetenv(argv[i], &g_shell->envp);
		ft_unsetenv(argv[i], &g_shell->expor);
		ft_unsetenv(argv[i], &g_shell->localp);
	}
	return (0);
}
