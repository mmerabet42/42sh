/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/12 11:58:28 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/25 16:13:02 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_str.h"
#include "ft_printf.h"
#include "ft_io.h"
#include "ft_mem.h"
#include <sys/stat.h>
#include <limits.h>
#include <stdio.h>

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

	if (!curpath || !*curpath || !(pwd = g_shell->pwd))
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
	char			*end;

	update_export("OLDPWD", g_shell->pwd);
	if (opt != 'P')
	{
		if ((*curpath = handle_opt_cd(curpath)))
			if (*(end = ft_strend(*curpath)) == '/' && *curpath != end)
				*end = '\0';
	}
	return (check_and_move_split(curpath, path));
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
	opt = 0;
	if (handle_opt(&argv, &opt))
		return (1);
	if (ft_strlen(*argv) >= PATH_MAX)
		return (!!ft_printf_fd(2, "42sh: cd: pathname too long\n"));
	if (!(curpath = get_curpath(*argv, &pathno)))
	{
		return (pathno ? 1 : !!ft_printf_fd(2, "42sh: cd: %s"
					": No such file or directory\n", *argv));
	}
	if (!(curpath = check_and_move(&curpath, *argv, opt)))
		return (1);
	getcwd(pwd, PATH_MAX);
	update_export("PWD", (opt == 'P' ? pwd : curpath));
	ft_strcpy(g_shell->pwd, (opt == 'P' ? pwd : curpath));
	pathno ? ft_putendl(curpath) : (void)pathno;
	ft_strdel(&curpath);
	return (0);
}
