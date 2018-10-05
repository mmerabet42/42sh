/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_end.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/15 23:32:59 by mmerabet          #+#    #+#             */
/*   Updated: 2018/10/05 11:18:56 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_str.h"

static void	del_func(void *content, size_t size)
{
	t_func	*func;

	(void)size;
	if ((func = (t_func *)content))
	{
		free(func->name);
		free(func->src);
		ft_astdel(&func->ast);
		free(func);
	}
}

static void	ft_free_tab(char ***tab)
{
	register int i;

	if (!tab && !*tab)
		return ;
	i = -1;
	while ((*tab)[++i])
		if ((*tab)[i])
			ft_strdel(&(*tab)[i]);
	free(*tab);
}


int			shell_end(void)
{
	int		exitcode;

	free(g_shell->script);
	ft_free_tab(&g_shell->paths);
	ft_free_tab(&g_shell->localp);
	ft_free_tab(&g_shell->envp);
	ft_free_tab(&g_shell->expor);
	clearhistory(1);
	free(g_shell->history_file);
	ft_lstdel(&g_shell->funcs, del_func);
	exitcode = g_shell->exitcode;
	free(g_shell);
	g_shell = NULL;
	return (exitcode);
}
