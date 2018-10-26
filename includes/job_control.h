/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_control.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/10 18:59:13 by mmerabet          #+#    #+#             */
/*   Updated: 2018/10/26 19:44:59 by sle-rest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOB_CONTROL_H
# define JOB_CONTROL_H

# include "shell.h"

# define MAX_BGPROC 21

typedef	enum		e_bgstat
{
	BG_OFFSET, BG_RUN, BG_END,
	BG_KILL, BG_STOP
}					t_bgstat;

typedef struct		s_bgstats
{
	int				status;
	char			*message;
}					t_bgstats;

# ifndef STRUCT_PIDS
#  define STRUCT_PIDS

typedef struct s_pids	t_pids;

struct				s_pids
{
	pid_t			pid;
	char			*cmd;
	t_pids			*parent;
	t_pids			*next;
};

# endif

typedef struct		s_inffork
{
	int				x;
	pid_t			pid;
	char			sign;
	int				status;
	char			*cmmd;
	unsigned int	modif : 1;
	t_pids			*pids;
}					t_inffork;

int					exec_cmd_background(t_ast *ast, void *res, t_iterf *iterf);
int					exec_btin_bin(t_ast *ast, void *res, t_iterf *iterf);
int					handle_bgstat(pid_t pid, int status, int opt);
int					handle_bgproc(pid_t pid_fork,
									char **cmd,
									int status,
									int opt);
int					check_bgend(void);
void				print_cmd_args2(char *tab);
char				**ret_args(t_ast *ast);
t_ast				*ret_astargs(t_ast *ast);
void				handle_bgsign(t_list *elem, int opt);
void				handle_pgid(void);
void				freelst_bg(void);
void				freetpids(t_pids **pids);
void				del(void *content, size_t size);
void				sign_child(int sign);
t_list				*ret_sign(int opt);
void				debug_sign();
void				resetsign(void);

int					creatpushelem(t_pids **head, pid_t pid, t_list *ast,
		int bquote);
void				extractpids(t_pids **head, pid_t pid);
t_list				*error_fg(char **argv, int i);

#endif
