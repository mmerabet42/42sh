/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ret_pipecmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 11:04:30 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/16 11:04:32 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_str.h"
#include "ft_mem.h"

static void	iter_astfillcmd(t_ast *ast, char *cmd)
{
	char			**tab;

	if (ast->left)
		iter_astfillcmd(ast->left, cmd);
	if (ast->name)
		ft_memcpy(&cmd[ft_strlen(cmd)], ast->name, ft_strlen(ast->name));
	if (ast->name && ast->args && (tab = ast->args->argv))
	{
		if (*tab && ++tab && *tab)
			cmd[ft_strlen(cmd)] = ' ';
		while (*tab)
		{
			ft_strcpy(&cmd[ft_strlen(cmd)], *tab);
			if (++tab && *tab)
				cmd[ft_strlen(cmd)] = ' ';
		}
	}
	if (ast->name)
		cmd[ft_strlen(cmd)] = ' ';
	if (ast->right)
		iter_astfillcmd(ast->right, cmd);
}

static void	iter_astsize(t_ast *ast, int *i)
{
	char	**tab;

	if (ast->left)
		iter_astsize(ast->left, i);
	if (ast->name)
		*i += (ft_strlen(ast->name) + 1);
	if (ast->name && ast->args && (tab = ast->args->argv))
	{
		*i += *tab ? 1 : 0;
		tab += *tab ? 1 : 0;
		while (*tab)
		{
			*i += (ft_strlen(*tab) + 1);
			tab++;
		}
		*i += 2;
	}
	if (ast->right)
		iter_astsize(ast->right, i);
}

int			ret_pipecmd(t_list *tabpipe, char **cmd)
{
	int		size;
	t_list	*tmp;

	(void)cmd;
	size = 0;
	tmp = ft_lstend(tabpipe);
	tabpipe = tmp;
	while (tabpipe)
	{
		iter_astsize((t_ast *)tabpipe->content, &size);
		tabpipe = tabpipe->parent;
		size += tabpipe ? 1 : 0;
	}
	if (!(*cmd = (char *)ft_memalloc(sizeof(char) * size)))
		return (SH_MALLOC);
	while (tmp)
	{
		iter_astfillcmd((t_ast *)tmp->content, *cmd);
		if ((tmp = tmp->parent))
			ft_strcpy(&(*cmd)[ft_strlen(*cmd)], "| ");
	}
	(*cmd)[(ft_strlen(*cmd) - 1)] = '\0';
	return (0);
}
