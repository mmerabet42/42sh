/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libedit.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 11:44:12 by gdufay            #+#    #+#             */
/*   Updated: 2018/10/23 20:04:14 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBEDIT_H
# define LIBEDIT_H

# include "shell.h"
# include "ft_str.h"
# include "ft_io.h"
# include "ft_mem.h"

# include <curses.h>
# include <term.h>
# include <sys/ioctl.h>
# include <unistd.h>
# include <sys/dir.h>
# include <sys/types.h>

/*
**	------------- TYPEDEF AND STRUCT -----------------
*/

typedef struct s_env	t_env;

typedef struct		s_cursor
{
	int x;
	int y;
	int xmax;
	int ymax;
	int origin;
}					t_cursor;

typedef struct		s_hst
{
	char			*hst;
	struct s_hst	*next;
	struct s_hst	*prev;
}					t_hst;

typedef struct		s_cmdedit
{
	char				c;
	int					is_autoc;
	struct s_cmdedit	*next;
	struct s_cmdedit	*prev;
}					t_cmdedit;

typedef struct		s_complete
{
	char				*name;
	int					head;
	struct s_complete	*next;
}					t_complete;

/*
**	------------- EDIT -----------------
*/

void				write_char(char c);
void				add_char(char *buf, t_cmdedit *cmd, t_cursor *cursor);
void				add_char_between(t_cmdedit *cmd, t_cursor cursor);
void				ft_free_t_cmdedit(t_cmdedit **cmd);
t_cmdedit			*handle_controld(t_cmdedit **cmd, t_cursor *cursor);
t_cmdedit			*end_of_text(t_cmdedit **cmd, t_cursor *cursor);
t_cmdedit			*create_cmdedit(char c);
t_cmdedit			*del_char(t_cmdedit **cmd, t_cursor *cursor);
t_cmdedit			*copy_edit(t_cmdedit **cmd, t_cmdedit **cp,
		t_cursor *cursor);
t_cmdedit			*paste_edit(t_cmdedit *cmd, t_cmdedit *cp,
		t_cursor *cursor);

/*
**	------------- UTILS -----------------
*/

t_cmdedit			*ft_main_loop(t_cmdedit *cmd, t_cursor *cursor);
char				*ft_loop_init(int prompt, int retry);
char				*list_to_str(t_cmdedit **cmd);
int					ft_new_term(void);
int					ft_clean_term(void);
int					init_cursor(int prompt, t_cursor *cursor);
int					ft_check_quote(char *s);
void				exec_t(char *s);
void				sighandler(int sig);
t_cmdedit			*ft_parser_edit(char *buf, t_cmdedit *cmd,
		t_cursor *cursor);
void				printprompt(int i);
char				**git_prompt(void);
int					free_git_prompt(char ***git_prompt, char **buf);
int					ft_putc(int c);
int					ft_isws(int c);

/*
**	------------- AUTOCOMPLETE -----------------
*/

enum {
	EXEC,
	VAR,
	OTHER
};

t_cmdedit			*autocomplete(t_cmdedit *cmd, t_cursor *cursor);
t_complete			*fill_complete_exec(char *path, char *cmp);
t_complete			*fill_complete_var(char *cmp);
t_complete			*fill_complete_other(char *path, char *cmp);
t_complete			*create_t_complete(char *name, t_complete *next);
void				t_completejoin(t_complete **head, t_complete *join);
char				*find_path(t_cmdedit *cmd);
char				*find_cmp(t_cmdedit *cmd);
void				add_t_complete(t_complete **head, t_complete **complete,
		char *name);
void				free_t_complete(t_complete **complete);
int					is_sep(char c);

/*
**	------------- HISTORY -----------------
*/

t_cmdedit			*mv_history(t_cmdedit **cmd, t_cursor *cursor, char c);

/*
**	------------- MOVE CURSOR -----------------
*/

t_cmdedit			*move_cursor(t_cmdedit *cmd, t_cursor *cursor, char mv);
void				mv_right(t_cursor *cursor);
void				mv_left(t_cursor *cursor);
t_cmdedit			*mv_multline(t_cmdedit *cmd, t_cursor *cursor, char mv);
t_cmdedit			*mv_by_word(t_cmdedit *cmd, t_cursor *cursor, char mv);
t_cmdedit			*mv_exterior(t_cmdedit *cmd, t_cursor *cursor, int touch);
void				mv_bnl(void);
void				mv_until_end(int len);

#endif
