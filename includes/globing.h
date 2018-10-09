/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sle-rest <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/22 21:40:35 by sle-rest          #+#    #+#             */
/*   Updated: 2018/10/09 14:01:01 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBING_H
# define GLOBING_H

# include <sys/dir.h>
# include "ft_list.h"
# include "parser.h"

# define G_START 0
# define G_END 1

enum				e_glob_type
{
	normal,
	stars,
	question,
	accolade,
	crochet
};

typedef struct		s_acc
{
	char			*str;
	int				type;
	char			**split;
	int				i_split;
	struct s_acc	*next;
	struct s_acc	*prev;
}					t_acc;

typedef struct		s_glob
{
	char			*pat;
	int				type;
	int				slash[2];
	struct s_glob	*next;
	struct s_glob	*prev;
}					t_glob;

int					ft_no_backslash(char *str, int i);

/*
**	------------- MALLOC_FREE_GLOB -----------------
*/

void				ft_free_tab(char ***tab);
int					malloc_glob(t_glob **glob);
int					malloc_acc(t_acc **acc);
void				free_acc(t_acc **acc);
void				free_splitpath(char ****splitpath);
void				free_triple_glob(t_glob ****glob);
int					error_glob(char *format, ...);

/*
**	------------- DETECT_GLOB -----------------
*/

int					detect_globbing_at(char *str, int i);
int					detect_globacc(char *str);
int					detect_globbing(char *str);

/*
**	------------- PARSING_ACC_PATH -----------------
*/

int					get_len_simplification(t_acc *acc, int i, int j);
int					len_between_comma(char *str, int i);
char				*get_elemacc_simplified(char *str, int *x, int y);
int					simplification_bracket(t_acc *acc);
int					get_struct_acc(char *str, t_acc **acc);
int					fill_acc_acc(char *str, int *i, t_acc **acc);
char				**split_acc(char *str);
int					refined_acc(t_acc *acc);
char				**get_splitacc(t_acc *acc);
char				***split_path(char **splitacc);

/*
**	------------- SETUP_GLOBBING -----------------
*/

int					last_char_splitpaacc_slash(char *splitacc);
int					loop_get_glob(char ***splitpath, t_glob ***glob,
		char **splitacc, int i);
int					setup_glob_normal(t_glob **glob, char *splitpath,
		char *splitacc, int *k);
int					setup_glob_stars(t_glob **glob, char *splitpath,
		char *splitacc, int *k);
int					setup_glob_question(t_glob **glob, char *splitpath,
		char *splitacc, int *k);
int					setup_glob_crochet(t_glob **glob, char *splitpath,
		char *splitacc, int *k);

/*
**	------------- GLOB_PROCESS -----------------
*/

int					exp_glob(t_strid *strid, t_list **lst, t_expf *expf);
int					ultimate_glob_process(t_glob **glob, t_list **lst,
		char **dir);
int					process_glob(t_glob ***glob, t_list **lst);

/*
**	------------- MATCH_GLOBBING -----------------
*/

int					split_check_match_stars(t_glob **glob, char *name, int *i);
int					is_match_stars(t_glob **glob, char *name, int *i);
int					is_match_glob(t_glob **glob, struct dirent *dp);
int					check_match_normal(t_glob **glob, char *name, int *i);
int					check_match_stars(t_glob **glob, char *name, int *i);
int					check_match_question(t_glob **glob, char *name, int *i);
int					check_match_crochet(t_glob **glob, char *name, int *i);

#endif
