/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/24 12:41:27 by jraymond          #+#    #+#             */
/*   Updated: 2018/10/25 16:45:20 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "parser.h"

int		type_pipe(t_ast *ast, void **op, void *res, t_iterf *iterf)
{
	(void)op;
	if (g_shell->bits & (1 << 2))
		return (shell_pipe_bg(ast, res, iterf));
	else if (g_shell->bits & (1 << 4))
		return (shell_pipe_bquote(ast, res, iterf));
	else
		return (shell_pipe_cb(ast, res, iterf));
}
