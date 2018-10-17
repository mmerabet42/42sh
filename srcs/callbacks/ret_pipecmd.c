#include "shell.h"
#include "ft_str.h"
#include "ft_mem.h"

static void	size_malloc(t_ast *ast, int *i)
{
	char	**tab;

	if (ast->left)
		size_malloc(ast->left, i);
	if (ast->name)
		*i += 1;
	if (ast->args && (tab = ast->args->argv))
	{
		tab += *tab ? 1 : 0;
		while (*tab)
		{
			*i += 1;
			tab++;
		}
	}
	if (ast->right)
		size_malloc(ast->right, i);
}

static void	fill_tabcmd(t_ast *ast, char **cmd)
{
	char	**arg;

	while (*cmd)
		cmd++;
	if (ast->name)
	{
		*cmd = ast->name;
		cmd++;
	}
	if (ast->args && (arg = ast->args->argv))
	{
		arg += *arg ? 1 : 0;
		while (*arg)
		{
			*cmd = *arg;
			cmd++;
			arg++;
		}
	}
}

int			ret_pipecmd(t_ast *ast, char ***cmd)
{
	int	size;

	size = 0;
	if (!*cmd)
	{
		size_malloc(ast, &size);
		if (!(*cmd = (char **)ft_memalloc(sizeof(char *) * (size + 1))))
			return (SH_MALLOC);
	}
	if (ast->left)
		ret_pipecmd(ast->left, cmd);
	fill_tabcmd(ast, *cmd);
	if (ast->right)
		ret_pipecmd(ast->right, cmd);
	return (0);
}

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

int			ret_pipecmd1(t_list *tabpipe, char **cmd)
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
	log_debug("'%s'\n", *cmd);
	exit (0);
	return (0);
}
