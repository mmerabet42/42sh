/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/04 16:31:11 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/04 16:39:43 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int					builtin_unset(int argc, char **argv)
{
	(void)argc;
	while (++argv)
	{
		ft_unsetenv(*argv, &g_shell->envp);
		ft_unsetenv(*argv, &g_shell->expor);
		ft_unsetenv(*argv, &g_shell->localp);
	}
	return (0);
}
