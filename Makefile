# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmerabet <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/01/11 18:07:15 by mmerabet          #+#    #+#              #
#    Updated: 2018/10/27 10:52:27 by sle-rest         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	42sh
CC			=	gcc
CFLAGS		=	-Wall -Werror -Wextra

LIBFTD		=	libft
LIBFT		=	$(LIBFTD)/libft.a
SRCD		=	srcs/

TERMCAPS	=	-ltermcap

INCLUDES	=	includes/expr.h includes/job_control.h includes/parser.h \
				includes/shell.h includes/globing.h \
				includes/libedit.h

_JBCNTRL_FS	=	check_bgend.c info_bg.c handle_bgsign.c handle_bgstatus.c lst.c \
				handle_pgid.c exec_cmd_background.c freelst_bg.c ret_astargs.c exec_btin_bin.c 
JBCNTRL_FS	=	$(addprefix $(SRCD)job_control/,$(_JBCNTRL_FS))
_JBCNTRL_FSO=	$(_JBCNTRL_FS:.c=.o)
JBCNTRL_FSO	=	$(JBCNTRL_FS:.c=.o)

_BLTN_FS	=	builtins.c builtin_cd.c builtin_echo.c builtin_setenv.c builtin_source.c \
				builtin_bool.c builtin_env.c builtin_jobs.c builtin_fg_error.c builtin_fg.c \
				builtin_bg.c ft_isbuiltin.c builtin_export.c builtin_set.c builtin_unset.c \
				builtin_read.c ft_split_whitespaces.c ft_multi_strjoin.c utils_cd.c \
				builtin_pwd.c builtin_utils.c get_curpath.c
BLTN_FS		=	$(addprefix $(SRCD)builtin/,$(_BLTN_FS))
_BLTN_FSO	=	$(_BLTN_FS:.c=.o)
BLTN_FSO	=	$(BLTN_FS:.c=.o)

_PARSER_FS	=	ft_parser.c ft_lexer.c ft_astiter.c ft_resolver.c ft_getopt.c \
				ft_argsdel.c ft_astdel.c newast.c ft_interpret.c utils.c ft_astresolver.c
PARSER_FS	=	$(addprefix $(SRCD)parser/,$(_PARSER_FS))
_PARSER_FSO	=	$(_PARSER_FS:.c=.o)
PARSER_FSO	=	$(PARSER_FS:.c=.o)

_SHCB_FS	=	shell_command.c shell_pipe.c shell_pipe_bg.c shell_arithmetic.c \
				shell_condition.c shell_equal.c shell_redir.c shell_redir1.c \
				shell_expansions.c shell_expansions1.c shell_localvar.c shell_error.c \
				shell_hdoc.c shell_seco.c handle_pipe.c ret_pipecmd.c shell_pipe_bquote.c \
				init_closedfd_swap_pipe.c type_pipe.c
SHCB_FS		=	$(addprefix $(SRCD)callbacks/,$(_SHCB_FS))
_SHCB_FSO	=	$(_SHCB_FS:.c=.o)
SHCB_FSO	=	$(SHCB_FS:.c=.o)

_EXPR_FS	=	ft_expr.c expr_callbacks.c expr_callbacks2.c
EXPR_FS		=	$(addprefix $(SRCD)expr/,$(_EXPR_FS))
_EXPR_FSO	=	$(_EXPR_FS:.c=.o)
EXPR_FSO	=	$(EXPR_FS:.c=.o)

_GLOB_FS	=	check_match_functions.c detect_globbing.c error_glob.c exp_glob.c fill_split_acc.c \
				free_functions_glob.c ft_no_backslash.c get_elemacc_simplified.c get_len_simplification.c \
				get_splitacc.c is_match_glob.c loop_get_glob.c malloc_functions_glob.c process_glob.c \
				setup_glob_crochet.c setup_glob_normal.c setup_glob_question.c setup_glob_stars.c \
				simplification_bracket.c split_acc.c split_path.c ultimate_glob_process.c
GLOB_FS		=	$(addprefix $(SRCD)globing/,$(_GLOB_FS))
_GLOB_FSO	=	$(_GLOB_FS:.c=.o)
GLOB_FSO	=	$(GLOB_FS:.c=.o)

_LIBEDIT_FS =	add_del_char.c check.c printprompt.c git_prompt.c \
				copy_paste.c edit_hst.c ft_loop.c init_libedit.c \
				list.c mv_by_word.c mv_cursor.c parser_edit.c signal.c \
				termcap.c autocomplete.c t_complete.c find_and_fill_complete.c \
				ft_main_loop.c
LIBEDIT_FS	=	$(addprefix $(SRCD)libedit/,$(_LIBEDIT_FS))
_LIBEDIT_FSO=	$(_LIBEDIT_FS:.c=.o)
LIBEDIT_FSO	=	$(LIBEDIT_FS:.c=.o)


_MAIN_FS	=	ft_env.c main.c ft_env2.c ft_getcursor.c ft_exec.c ft_getpaths.c sign_chld.c \
				ft_parsepath.c list_redirections.c history.c shell_init.c shell_begin.c shell_end.c
MAIN_FS		=	$(addprefix $(SRCD)main/,$(_MAIN_FS))
_MAIN_FSO	=	$(_MAIN_FS:.c=.o)
MAIN_FSO	=	$(MAIN_FS:.c=.o)

ICLD		=	-Iincludes -I$(LIBFTD)/includes
SRCS		=	$(MAIN_FS) $(LIBEDIT_FS) $(JBCNTRL_FS) $(BLTN_FS) \
				$(PARSER_FS) $(SHCB_FS) $(EXPR_FS) $(GLOB_FS)
_OBJS		=	$(_MAIN_FSO) $(_LIBEDIT_FSO) $(_JBCNTRL_FSO) $(_BLTN_FSO) \
				$(_PARSER_FSO) $(_SHCB_FSO) $(_EXPR_FSO) $(_GLOB_FSO)
OBJD		=	.objs/
OBJS		=	$(addprefix $(SRCD),$(_OBJS))
OBJB		=	$(addprefix $(OBJD),$(_OBJS))

FRAMEWORKS	=

# COLORS
CRED=\033[91m
CGREEN=\033[38;2;0;255;145m
CEND=\033[0m

all: 
	@$(MAKE) lib
	@$(MAKE) $(NAME)

$(NAME): $(LIBFT) $(OBJB)
	@printf "\r\033[K$(CGREEN)Creating executable$(CEND): $(NAME)\n"
	@$(CC) $(CFLAGS) $(OBJB) $(LIBFT) $(TERMCAPS) $(FRAMEWORKS) -o $(NAME)
	@echo  "$(NAME): $(CGREEN)done$(CEND)"

lib:
	@make -C $(LIBFTD)

$(OBJD)%.o: $(SRCD)main/%.c $(INCLUDES) Makefile
	@printf "\r\033[K$(CGREEN)Compiling$(CEND): $<"
	@mkdir -p $(OBJD)
	@$(CC) $(CFLAGS) -o $@ -c $< $(ICLD)

$(OBJD)%.o: $(SRCD)job_control/%.c $(INCLUDES) Makefile
	@printf "\r\033[K$(CGREEN)Compiling$(CEND): $<"
	@mkdir -p $(OBJD)
	@$(CC) $(CFLAGS) -o $@ -c $< $(ICLD)

$(OBJD)%.o: $(SRCD)builtin/%.c $(INCLUDES) Makefile
	@printf "\r\033[K$(CGREEN)Compiling$(CEND): $<"
	@mkdir -p $(OBJD)
	@$(CC) $(CFLAGS) -o $@ -c $< $(ICLD)

$(OBJD)%.o: $(SRCD)parser/%.c $(INCLUDES) Makefile
	@printf "\r\033[K$(CGREEN)Compiling$(CEND): $<"
	@mkdir -p $(OBJD)
	@$(CC) $(CFLAGS) -o $@ -c $< $(ICLD)

$(OBJD)%.o: $(SRCD)callbacks/%.c $(INCLUDES) Makefile
	@printf "\r\033[K$(CGREEN)Compiling$(CEND): $<"
	@mkdir -p $(OBJD)
	@$(CC) $(CFLAGS) -o $@ -c $< $(ICLD)

$(OBJD)%.o: $(SRCD)expr/%.c $(INCLUDES) Makefile
	@printf "\r\033[K$(CGREEN)Compiling$(CEND): $<"
	@mkdir -p $(OBJD)
	@$(CC) $(CFLAGS) -o $@ -c $< $(ICLD)

$(OBJD)%.o: $(SRCD)globing/%.c $(INCLUDES) Makefile
	@printf "\r\033[K$(CGREEN)Compiling$(CEND): $<"
	@mkdir -p $(OBJD)
	@$(CC) $(CFLAGS) -o $@ -c $< $(ICLD)

$(OBJD)%.o: $(SRCD)libedit/%.c $(INCLUDES) Makefile
	@printf "\r\033[K$(CGREEN)Compiling$(CEND): $<"
	@mkdir -p $(OBJD)
	@$(CC) $(CFLAGS) -o $@ -c $< $(ICLD)

clean:
	@make -C $(LIBFTD) clean
	@echo "$(CRED)Cleaning$(CEND): $(NAME)"
	@rm -f $(OBJB)

fclean: clean
	@make -C $(LIBFTD) fclean
	@echo "$(CRED)Full cleaning$(CEND): $(NAME)"
	@/bin/rm -f $(NAME)

re:
	@make fclean
	@make

.PHONY: all clean fclean re
