/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/23 19:59:17 by mmerabet          #+#    #+#             */
/*   Updated: 2018/08/23 20:01:42 by mmerabet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PARSING_H
# define FT_PARSING_H

# include <stddef.h>
# include "ft_list.h"

# define RGX_END (1 << 0)
# define RGX_RGXN (1 << 1)
# define RGX_STRN (1 << 2)
# define RGX_POS (1 << 3)
# define RGX_MATCHES (1 << 4)
# define RGX_VAR (1 << 5)

enum				e_regex_condtion
{
	RGX_GREAT = 1, RGX_LESS = 2, RGX_EQUAL = 3, RGX_MARK = 4
};

typedef struct		s_regex_rule
{
	const char		*rule;
	const char		*arg;
	int				len_arg;
	int				len_rule;
	int				cond;
	int				l;
	char			type;
	int				neg;
	int				i;
}					t_regex_rule;

typedef struct		s_regex_info
{
	const char		*param;
	const char		*str_begin;
	const char		*rgx_begin;
	const char		*str;
	const char		*regex;
	const char		*next;
	int				*vars;
	int				len_param;
	int				param_i;
	int				len;
	int				rgxn;
	int				strn;
	int				option;
	int				*pos;
	t_list			**matches;
}					t_regex_info;

typedef int	(*t_regex_funcptr)(t_regex_info *regex_info, t_regex_rule *rule);
typedef struct		s_regex_func
{
	char			*name;
	t_regex_funcptr	func;
}					t_regex_func;


int					cond_rgx(t_regex_info *rgxi, t_regex_rule *rule);
int					expr_rgx(t_regex_info *rgxi, t_regex_rule *rule);
int					print_rgx(t_regex_info *rgxi, t_regex_rule *rule);
int					getint_rgx(t_regex_info *rgxi, t_regex_rule *rule);
int					debug_rgx(t_regex_info *rgxi, t_regex_rule *rule);
int					recursive_rgx(t_regex_info *rgxi, t_regex_rule *rule);
int					group_rgx(t_regex_info *rgxi, t_regex_rule *rule);
t_regex_func		*get_regex_func(const char *name, int len_rule);

int					regex_variable(t_regex_info *rgxi, const char *s);
int					regex_start(t_regex_info *rgxi, t_regex_func *func, t_regex_rule *rule);
int					regex_wildcard(t_regex_info *rgxi);
int					regex_bracket(const char *str, int *s);
int					regex_exec(t_regex_info *regex_info);
void				regex_init(t_regex_info *regex_info, const char *regex, const char *str);

int					ft_regex(int options, const char *regex, const char *str, ...);

#endif
