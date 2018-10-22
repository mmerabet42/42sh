/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_interpret.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/10 21:35:11 by mmerabet          #+#    #+#             */
/*   Updated: 2018/10/22 20:06:17 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_printf.h"

int			ft_interpret(const char *cmd, t_lexerf *lexf,
					t_iterf *itf)
{
	t_ast	*head;
	int		ret;

	(void)itf;
	ret = -1;
	if (!(head = ft_lexer(cmd, lexf)))
		return (0);
	if ((ret = check_syntax(head, (t_allf *)lexf->data, 0)))
	{
		ft_astdel(&head);
		return (ret);
	}
	ret = ft_astiter(head, &g_shell->exitcode, itf);
	if (ret && ret != SH_EXIT)
		ft_printf_fd(2, "%s: %s: [%s]\n", g_shell->name, ft_strshret(ret),
				cmd, (g_shell->exitcode = 1));
	ft_astdel(&head);
	return (ret);
}
