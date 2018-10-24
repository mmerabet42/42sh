/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printprompt.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/10 20:10:31 by mmerabet          #+#    #+#             */
/*   Updated: 2018/10/24 18:09:47 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "ft_str.h"
#include "ft_printf.h"
#include "libedit.h"

static void	usualprompt(void)
{
	static int	e = 50;
	static int	rev = 0;
	char		*start_pwd;
	char		**git;

	start_pwd = NULL;
	git = git_prompt();
	if (ft_strstr_pos(g_shell->pwd, g_shell->homepwd) == 0)
		start_pwd = g_shell->pwd + ft_strlen(g_shell->homepwd);
	ft_printf("%{0}%{bold}%C%{0} %{lred}%s %{lcyan}%s%s%{0}"
			" %{lblue}%s%{red}%s%{lblue}%s",
			L'㋜', (g_shell->user ? g_shell->user : "42sh"),
			(start_pwd ? "~" : ""), (start_pwd ? start_pwd : g_shell->pwd),
			git ? git[0] : "", git ? git[1] : "", git ? git[2] : "");
	if (g_shell->exitcode)
		ft_printf("%{bold/lred}(%d)%C%{0} ", g_shell->exitcode, L'❯');
	else
		ft_printf("%l{%1$d;%1$d;%1$d}%{bold}%C%{0} ", (e += (rev ? -10 : 10)),
			L'❯');
	if (rev && e <= 50)
		rev = 0;
	if (!rev && e >= 250)
		rev = 1;
	free_git_prompt(&git, NULL);
}

static t_exp		g_exps[] = {
	{"\\\\*[@=1]", exp_var},
	{"$*[aA0_-zZ9_]:$?", exp_var},
	{"$*[0-9]:$#:$@:$@*[0-9]", exp_arg},
	{"*[$((?));(?);\"*\";'*'@b]", exp_arth},
	{"~", exp_tild},
	{"*[${?};\"*\";'*'@b]", exp_dvar},
	{"*[$(?);(?);`?`;${?};\"*\";'*'@b]", exp_cmd},
	{"*[`?`;$(?);${?};\"*\";'*'@b]", exp_cmd},
	{"*[$\\[*\\];\"*\";'*'@b]", exp_cond},
};

static t_expf		g_expf = {
	g_exps, sizeof(g_exps), NULL, 0
};

void		printprompt(int i)
{
	char	*prompt;
	t_list	*nprompt;

	if (i)
	{
		ft_getcursor(&i, NULL);
		if (i > 1)
			ft_printf("%{0}%{invert}%%%{0}\n");
	}
	if ((prompt = ft_getenv("PROMPT", g_shell->envp)))
	{
		nprompt = NULL;
		g_expf.data = g_shell->allf;
		ft_strexpand(prompt, &nprompt, -1, &g_expf);
		if (nprompt && nprompt->content)
			ft_printf(nprompt->content);
		ft_lstdel(&nprompt, content_delfunc);
	}
	else
		usualprompt();
}
