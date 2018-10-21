/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/12 11:58:28 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/20 17:53:45 by ouralgan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_str.h"
#include "ft_printf.h"
#include "ft_io.h"
#include "ft_mem.h"
#include <sys/stat.h>
#include <limits.h>

static char	*canonical(char *curpath, char *canon)
{
	unsigned int	component;
	struct stat		buf;
	char			*tmp;

	if (!canon || !curpath || !*curpath)
		return (canon);
	tmp = ft_strchr(curpath, '/');
	component = (tmp ? (unsigned int)(tmp - curpath) : ft_strlen(curpath));
	if (!component && ft_strlen(canon) > 0
			&& canon[ft_strlen(canon) - 1] == '/')
		return (canonical(curpath + component + 1, canon));
	if (component && !ft_strncmp(curpath, "./", component))
		return (canonical(curpath + component * 2 - 1, canon));
	if (component && !ft_strncmp(curpath, "../", component))
	{
		if (stat(canon, &buf) == -1 || (buf.st_mode & S_IFMT) != S_IFDIR)
			return (NULL);
		canon = remove_prec_component(canon);
		return (canonical(curpath + component, canon));
	}
	canon = ft_strncat(canon, curpath, component + 1);
	return (canonical(curpath + component, canon));
}

static char	*handle_opt_cd(char **curpath)
{
	char	*canon;
	char	*tmp;
	char	*pwd;
	size_t	len;
	char	pwd_buf[PATH_MAX];

	if (!curpath || !*curpath || !(pwd = ft_getenv("PWD", g_shell->envp)))
	{
		update_export("PWD", getcwd(pwd_buf, PATH_MAX));
		ft_strdel(curpath);
		return (NULL);
	}
	len = ft_strlen(pwd);
	if (*(*curpath) != '/' && pwd[len - 1] != '/')
		tmp = ft_multi_strjoin(3, pwd, "/", *curpath);
	else if (**curpath != '/' && pwd[len - 1] == '/')
		tmp = ft_multi_strjoin(2, pwd, *curpath);
	else
		tmp = ft_strdup(*curpath);
	canon = ft_strnew(ft_strlen(tmp));
	ft_strdel(curpath);
	if (!(*curpath = canonical(tmp, canon)))
		ft_strdel(&canon);
	ft_strdel(&tmp);
	return (*curpath);
}

static char	*check_and_move(char **curpath, char *path, char opt)
{
	struct stat		buf;
	char			pwd[PATH_MAX];

	update_export("OLDPWD", getcwd(pwd, PATH_MAX));
	if (opt != 'P')
		*curpath = handle_opt_cd(curpath);
	if (stat((*curpath), &buf) == -1)
	{
		ft_strdel(curpath);
		return ((void*)(size_t)!ft_printf("42sh: cd: %s: No such file or direc"
					"tory\n", path ? path : ft_getenv("HOME", g_shell->envp)));
	}
	if ((buf.st_mode & S_IFMT) != S_IFDIR)
	{
		ft_strdel(curpath);
		return ((void*)(size_t)!ft_printf("42sh: cd: %s: Not a directory\n",
					path ? path : ft_getenv("HOME", g_shell->envp)));
	}
	if (chdir(*curpath) == -1)
	{
		ft_strdel(curpath);
		return ((void*)(size_t)!ft_printf("42sh: cd: %s: Permission denied\n",
					path ? path : ft_getenv("HOME", g_shell->envp)));
	}
	return (*curpath);
}

static int	handle_opt(char ***argv, char *p)
{
	t_opt	opt;

	ft_bzero(&opt, sizeof(t_opt));
	while (ft_getopt(argv, "LP", &opt) != OPT_END)
	{
		if (opt.c == 'L' || opt.c == 'P')
			*p = opt.c;
		else if (opt.c == '\0' && (*argv = opt.ptr - 1))
			break ;
		else if (ft_printf_fd(2, "cd: bad option: %c\n", opt.c))
			return (1);
	}
	return (0);
}

int			builtin_cd(int argc, char **argv)
{
	int				pathno;
	char			opt;
	char			*curpath;
	char			pwd[PATH_MAX];

	pathno = argc - argc;
	argv++;
	if (handle_opt(&argv, &opt))
		return (1);
	if (ft_strlen(*argv) >= PATH_MAX)
		return (!!ft_printf("42sh: cd: pathname too long\n"));
	if (!(curpath = get_curpath(*argv, &pathno)))
	{
		return (pathno ? 1 : !!ft_printf("42sh: cd: %s"
					": No such file or directory\n", *argv));
	}
	if (!(curpath = check_and_move(&curpath, *argv, opt)))
		return (1);
	getcwd(pwd, PATH_MAX);
	update_export("PWD", (opt == 'P' ? pwd : curpath));
	ft_strcpy(g_shell->pwd, (opt == 'P' ? pwd : curpath));
	if (pathno)
		ft_putendl(curpath);
	ft_strdel(&curpath);
	return (0);
}
