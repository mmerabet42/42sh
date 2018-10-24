/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/24 12:41:27 by jraymond          #+#    #+#             */
/*   Updated: 2018/10/24 12:58:16 by jraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "parser.h"

int		type_pipe(t_ast *ast, void **op, void *res, t_iterf *iterf)
{
	(void)op;
	check_syntax(ast, (t_allf *)iterf->data, 1);
	if (g_shell->bits & (1 << 2))
		return (shell_pipe_bg(ast, res, iterf));
	else if (g_shell->bits & (1 << 4))
		return (shell_pipe_bquote(ast, res, iterf));
	else
		return (shell_pipe_cb(ast, res, iterf));
}
