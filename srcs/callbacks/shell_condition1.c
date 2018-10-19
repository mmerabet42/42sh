/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_condition1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 11:04:48 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/16 11:10:27 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	shell_then_cb(t_ast *ast, void **op, void *res, t_iterf *iterf)
{
	int	efail;

	res2 = 0;
	if (!ft_astgood(ast))
		return (SH_BADEXPR);
	if (!ast->parent || !ast->parent->name
			|| !ft_strequ(ast->parent->name, "if")
			|| !ft_strequ(ast->parent->name, "while"))
		return (SH_THENWCOND);
	if ((efail = ft_astiter(ast->left, res, iterf)))
		return (efail);
	if (*(int *)res)
		*(int *)res = (*(int *)res << 1) | 1;
	else
	{
		if (efail = ft_astiter(ast->right, res, iterf))
			return (efail);
		*(int *)res <<= 1;
	}
	return (0);
}

int	shell_if_cb(t_ast *ast, void **op, void *res, t_iterf *iterf)
{
	if (!ast->right || !ast->right->name || ast->right->type == ast->cmd_offset
			|| !ft_strequ(ast->right->name, "else")
			|| !ft_strequ(ast->right->name, "then"))
		return (SH_CONDWTHEN);
}
