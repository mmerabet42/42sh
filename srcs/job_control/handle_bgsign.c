/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_bgsign.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jraymond <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/01 14:51:21 by jraymond          #+#    #+#             */
/*   Updated: 2018/10/26 17:25:08 by jraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** handle value g_shell->lastmodif_pid;
** handle sign + and - for the background;
** opt true => the bg is finish;
** opt false => the bg are new sign +;
*/

#include "shell.h"
#include "ft_printf.h"

t_list			*ret_sign(int opt)
{
	t_list	*lst;

	lst = g_shell->bgproc;
	if (opt)
	{
		while (lst && ((t_inffork *)lst->content)->sign != '+')
			lst = lst->next;
	}
	else
	{
		while (lst && ((t_inffork *)lst->content)->sign != '-')
			lst = lst->next;
	}
	return (lst);
}

static	int		no_opt(t_list *elem)
{
	t_list	*lst;
	t_list	*ret;

	lst = g_shell->bgproc;
	while (lst)
	{
		if (((t_inffork *)lst->content)->sign == '+' && lst != elem)
		{
			if ((ret = ret_sign(0)))
				((t_inffork *)ret->content)->sign = ' ';
			((t_inffork *)lst->content)->sign = '-';
			((t_inffork *)elem->content)->sign = '+';
			g_shell->lstmodif_pid = elem;
			return (0);
		}
		else if (lst == elem && ((t_inffork *)lst->content)->sign == '+')
			return (0);
		lst = lst->next;
	}
	return (0);
}

static void		handle_signmore(t_list *elem)
{
	t_list	*lst;

	if (!(lst = ret_sign(0)))
		return ;
	((t_inffork *)lst->content)->sign = '+';
	g_shell->lstmodif_pid = lst;
	if (elem->parent && ((t_inffork *)elem->parent->content)->sign == ' ')
		((t_inffork *)elem->parent->content)->sign = '-';
	else if (elem->next && ((t_inffork *)elem->next->content)->sign == ' ')
		((t_inffork *)elem->next->content)->sign = '-';
	else if (lst->parent && ((t_inffork *)lst->parent->content)->sign == ' ')
		((t_inffork *)lst->parent->content)->sign = '-';
	else if (lst->next && ((t_inffork *)lst->next->content)->sign == ' ')
		((t_inffork *)lst->next->content)->sign = '-';
}

static	int		withopt(t_list *elem)
{
	char	c;

	c = ((t_inffork *)elem->content)->sign;
	if (c == '-')
	{
		if (elem->parent)
			((t_inffork *)elem->parent->content)->sign = '-';
		else if (elem->next)
			((t_inffork *)elem->next->content)->sign = '-';
	}
	else if (c == '+')
		handle_signmore(elem);
	return (0);
}

void			handle_bgsign(t_list *elem, int opt)
{
	int	ret;

	if (!elem)
		return ;
	ret = ft_lstsize(g_shell->bgproc);
	if (ret == 1)
	{
		if (!opt)
			((t_inffork *)elem->content)->sign = '+';
		else
			return ;
	}
	else
	{
		if (opt)
			withopt(elem);
		else
			no_opt(elem);
	}
}
