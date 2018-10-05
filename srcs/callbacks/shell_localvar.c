/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_localvar.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/03 16:33:57 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/04 15:33:07 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_printf.h"
#include "ft_str.h"

int			shell_equal(t_ast *ast, void **op, void *res, t_iterf *iterf)
{
	char	name[1024];
	char	*content;
	size_t	len;

	(void)op;
	ft_astcresolver(ast, ((t_allf *)iterf->data)->expf);
	content = ft_strchr(ast->cname, '=');
	len = content - ast->cname;
	ft_strncpy(name, ast->cname, len < 1024 ? len : 1023);
	name[len < 1024 ? len : 1023] = 0;
	if (ft_getenv(name, g_shell->expor))
		update_export(name, content + 1);
	else
		ft_setenv(name, content + 1, &g_shell->localp);
	*(int *)res = 0;
	return (0);
}
