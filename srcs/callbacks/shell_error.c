/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/17 21:52:09 by mmerabet          #+#    #+#             */
/*   Updated: 2018/09/17 22:58:48 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_str.h"
#include <fcntl.h>
#include "ft_types.h"
#include "ft_io.h"

static int		read_heredoc(int fd, char *hdoc_file, t_ast *ast, char *ptr)
{
	char	line[500];
	int		c;

	while (ft_strclr(line))
	{
		ft_printf("heredoc> ");
		c = ft_readraw(line, 500);
		ft_putchar('\n');
		if (c == 3 || c == 4)
		{
			free(hdoc_file);
			close(fd);
			return (c);
		}
		if (ft_strequ(line, ast->right->name))
			break ;
		ft_putendl_fd(line, fd);
	}
	ptr[1] = '\0';
	free(ast->right->name);
	ast->right->name = hdoc_file;
	ast->right->args->argv[0] = ast->right->name;
	close(fd);
	return (0);
}

static int	repair_hdoc(t_ast *ast, void **op, void *res, t_iterf *iterf)
{
	char		*ptr;
	char		*hdoc_file;
	int			ret[2];
	int			n;

	if (!ast || ast->type == ast->cmd_offset)
		return (0);
	if (!(n = (*(int *)res)++) && (!ast->left || !ast->left->name))
		return (1);
	if ((ret[0] = repair_hdoc(ast->left, op, res, iterf)) == 3)
		return (ret[0]);
	*(int *)res = n + 2;
	if ((ret[1] = repair_hdoc(ast->right, op, res, iterf)) == 3)
		return (ret[1]);
	ptr = ft_strstr(ast->name, "<<");
	if (ast->right && ast->right->name)
	{
		if (!(hdoc_file = ft_strjoin_clr(HDOC_TMP_FILE, ft_itoa(n), 1)))
			return (0);
		if ((n = open(hdoc_file, O_WRONLY | O_TRUNC | O_CREAT, 0666)) != -1)
			return (read_heredoc(n, hdoc_file, ast, ptr));
		free(hdoc_file);
	}
	else
		return (1);
	if (ret[0])
		return (ret[0]);
	return (ret[1]);
}

static int	shell_error_cb(t_ast *ast, void **op, void *res, t_iterf *iterf)
{
	(void)op;
	(void)res;
	(void)iterf;
	if (!ast->left || !ast->left->name)
		return (2);
	else if (*ast->name != ';' && !ft_strequ(ast->name, "&")
			&& (ast->type != TK_REDIR || !ft_strchr(ast->name, '&'))
			&& (!ast->right || !ast->right->name))
		return (2);
	return (0);
}

static t_astfunc	g_callbacks[] = {
	{"&&:||:|:&:;", shell_error_cb, NULL, -2},
	{"*<<*", NULL, repair_hdoc, 0},
	{DLM_REDP, shell_error_cb, NULL, -2}
};

static t_iterf		g_iterf = {
	g_callbacks, sizeof(g_callbacks) / sizeof(t_astfunc), NULL, 0, sizeof(int)
};

int	check_syntax(t_ast *ast)
{
	int	res;

	res = 0;
	return (ft_astiter(ast, &res, &g_iterf));
}
