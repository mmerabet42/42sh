#include "shell.h"
#include "ft_str.h"
#include "ft_mem.h"

static void	size_malloc(t_ast *ast, int *i)
{
	char	**tab;

	if (ast->left)
		size_malloc(ast->left, i);
	if (ast->name)
	{
		log_debug("name -> '%s'\n", ast->name);
		*i += 1;
	}
	if (ast->args && (tab = ast->args->argv))
	{
		tab += *tab ? 1 : 0;
		while (*tab)
		{
			log_debug("args -> '%s'\n", *tab);
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
		log_debug("size_tab: %d\n", size);
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
