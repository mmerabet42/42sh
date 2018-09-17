/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_glob.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/02 22:30:11 by sle-rest          #+#    #+#             */
/*   Updated: 2018/09/17 22:23:12 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"
#include "parser.h"
# include "ft_list.h"
#include "ft_str.h"
#include "shell.h"

// FONCTION DE TEST
/*
static void	print_glob(t_glob *glob)
{
	while (glob)
	{
		dprintf(1, "glob->pat: %s\n", glob->pat);
		dprintf(1, "glob->type: %d\n", glob->type);
		dprintf(1, "glob->slash[0]: %d glob->slash[1]: %d\n", glob->slash[0], glob->slash[1]);
		glob = glob->next;
	}
}

static void	print_triple_glob(t_glob ***glob)
{
	int	i = 0;
	int	j;
	while (glob && glob[i])
	{
		j = 0;
		while (glob && glob[i][j])
		{
			dprintf(1, "------------------\n");
			dprintf(1, "print_glob[%d][%d]\n", i, j);
			print_glob(glob[i][j]);
			j++;
		}
		i++;
	}
}
*/
static t_glob	***get_glob(char ***splitpath, char **splitacc)
{
	t_glob	***glob;
	int		len;
	int		i;

	len = 0;
	while (splitpath[len])
		len++;
	if (!(glob = (t_glob ***)malloc(sizeof(t_glob **) * (len + 1))))
		return (NULL);
	i = -1;
	while (splitpath[++i])
	{
		if (!loop_get_glob(splitpath, glob, splitacc, i))
		{
			error_glob("g", &glob);
			return (NULL);
		}
	}
	glob[i] = NULL;
	return (glob);
}

static int		split_splitacc_into_lst(t_list **lst, t_list *tmp,
											char **splitacc)
{
	int		i;
	t_list	*new;

	i = 0;
	while (splitacc[++i])
	{
		if (!(new = ft_lstnew(splitacc[i], ft_strlen(splitacc[i]) + 1)))
			return (0);
		new->next = (*lst)->next;
		if ((*lst)->next)
			(*lst)->next->parent = new;
		(*lst)->next = new;
		new->parent = *lst;
		*lst = new;
	}
	while (*lst && (*lst)->parent && (*lst)->parent != tmp)
		*lst = (*lst)->parent;
	return (1);
}

static int		splitacc_into_lst(t_list **lst, char **splitacc)
{
	t_list	*new;

	if (!(new = ft_lstnew(splitacc[0], ft_strlen(splitacc[0]) + 1)))
		return (0);
	new->next = (*lst)->next;
	if ((*lst)->next)
		(*lst)->next->parent = new;
	(*lst)->next = new;
	free((*lst)->content);
	free(*lst);
	*lst = new;
	if (!split_splitacc_into_lst(lst, *lst, splitacc))
		return (0);
	return (1);
}

static int		get_match(t_list **lst)
{
	char	**splitacc;
	char	***splitpath;
	t_glob	***glob;

	if (!(splitacc = split_acc((*lst)->content)))
		return (0);
	if (!(splitpath = split_path(splitacc)))
		return (error_glob("t", &splitacc)); // NORMALEMENT ok
	if (!splitacc_into_lst(lst, splitacc))// NORMALEMENT ok
		return (error_glob("Tt", &splitpath, &splitacc)); // NORMALEMENT ok
	if (!(glob = get_glob(splitpath, splitacc)))
		return (error_glob("Tt", &splitpath, &splitacc)); // NORMALEMENT ok
//	print_triple_glob(glob);
	ft_free_tab(&splitacc);
	free_splitpath(&splitpath); // Surement pas doublon mais on sait jamais
	if (!process_glob(glob, lst)) // NORMALEMENT ok
		return (error_glob("Ttg", &splitpath, &splitacc, &glob));// NORMALEMENT ok
	free_triple_glob(&glob);
	return (1);
}

int				exp_glob(t_strid *strid, t_list **lst, t_expf *expf)
{
	(void)expf;
	if (!strid->i)
		return (0);
	if (!(*lst = (t_list *)malloc(sizeof(t_list))))
		return (SH_MALLOC);
	if (!((*lst)->content = ft_strdup(strid->str)))
		return (0);
	(*lst)->content_size = ft_strlen((*lst)->content);
	(*lst)->next = NULL;
	(*lst)->parent = NULL;
	if (detect_globbing((*lst)->content)) // a modifier
	{
		if (!get_match(lst))
			return (SH_MALLOC); // ??
	}
	while (lst && *lst && (*lst)->parent)
		*lst = (*lst)->parent;
	return (0);
}
/*
t_list	*first;
first = *lst;
while (first)
{
	dprintf(1, "lst->content %s\n", first->content);
	dprintf(1, "lst->content_size %zu\n", first->content_size);
	first = first->next;
}

typedef struct		s_list
{
	void			*content;
	size_t			content_size;
	struct s_list	*next;
	struct s_list	*parent;
}					t_list;

typedef strcut		s_args
{
	char	*arg;
	t_args	*next;
	t_args	*prev;
}					t_args;

typedef struct		s_strid
{
	char			*str;
	char			*next;
	int				len;
	int				ifound;
	int				jump;
	int				i;
	int				j;
}					t_strid;

typedef struct		s_expf
{
	t_exp			*expansions;
	size_t			len;
	void			*data;
	int				onlyfirst;
}					t_expf;

typedef struct		s_exp
{
	char			*name;
	t_expfunc		func;
}					t_exp;
*/
