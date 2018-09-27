/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/18 15:35:12 by gdufay            #+#    #+#             */
/*   Updated: 2018/02/12 10:30:25 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "../libft.h"
# include <stdarg.h>
# include <wchar.h>

int					ft_printf(const char *format, ...);
typedef	struct		s_option
{
	char	*flag;
	int		prec;
	int		field;
	char	*arg;
	char	carac;
	int		up;
	int		base;
	int		is_neg;
}					t_option;
typedef	int		(*t_fonction)(va_list va, char
		**format, t_option* option);
typedef	struct		s_conv
{
	char		elem;
	t_fonction	f;
}					t_conv;
typedef	char*	(*t_ft)(va_list va, char
		**format, t_option **option);
typedef	struct		s_long
{
	char	*elem;
	t_ft	f;
}					t_long;
int					ft_s(va_list va, char **format, t_option *option);
int					ft_lstring(va_list va, char **format, t_option *option);
int					ft_c(va_list va, char **format, t_option *option);
int					ft_lc(va_list va, char **format, t_option *option);
int					ft_mod(va_list va, char **format, t_option *option);
int					ft_p(va_list va, char **format, t_option *option);
int					ft_signed(va_list va, char **format, t_option *option);
char				*ft_hhu(va_list va, char **format, t_option **option);
char				*ft_hu(va_list va, char **format, t_option **option);
char				*ft_lu(va_list va, char **format, t_option **option);
char				*ft_llu(va_list va, char **format, t_option **option);
char				*ft_ju(va_list va, char **format, t_option **option);
char				*ft_zu(va_list va, char **format, t_option **option);
char				*ft_u(va_list va, char **format, t_option **option);
char				*ft_hh(va_list va, char **format, t_option **option);
char				*ft_h(va_list va, char **format, t_option **option);
char				*ft_l(va_list va, char **format, t_option **option);
char				*ft_ll(va_list va, char **format, t_option **option);
char				*ft_j(va_list va, char **format, t_option **option);
char				*ft_z(va_list va, char **format, t_option **option);
char				*ft_d(va_list va, char **format, t_option **option);
int					ft_unsigned(va_list va, char **format, t_option *option);
void				ft_check_flag(t_option **option, char **format);
void				ft_check_field(t_option **option, char **format);
void				ft_check_precision(t_option **option, char **format);
void				ft_clean_option(t_option **option);
char				*ft_malloc(t_option *option, int len);
int					ft_intfill(t_option *option);
int					ft_charfill(t_option *option);
void				ft_fill_int_left(char **tabi, t_option *option,
		int zero, char flag);
void				ft_fill_int_right(char **tabi, t_option *option, char flag);
void				ft_fill_char_right(char **tabi, t_option *option);
void				ft_fill_char_left(char **tabi, t_option *option, int zero);

#endif
