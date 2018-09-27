.PHONY: all fclean clean re
NAME = libft.a

FT_PRINTF = check_options.c conv1.c conv2.c conv3.c fill.c ft_lstring.c ft_printf.c modif.c type_char.c type_int.c

LIBFTPRINTF_SRC = libftprintf/{check_options.c,conv1.c,conv2.c,conv3.c,fill.c,ft_lstring.c,ft_printf.c,modif.c,type_char.c,type_int.c}

LIBFTPRINTF_OBJ = $(FT_PRINTF:.c=.o)

SOURCE = ft_atoi.c\
		 ft_bzero.c\
		 ft_charswap.c\
		 ft_isalnum.c\
		 ft_countwords.c\
		 ft_isascii.c\
		 ft_isalpha.c\
		 ft_isdigit.c\
		 ft_isprint.c\
		 ft_isws.c\
		 ft_lstadd.c\
		 ft_lstdel.c\
		 ft_lstdelone.c\
		 ft_lstiter.c\
		 ft_memalloc.c\
		 ft_lstmap.c\
		 ft_lstnew.c\
		 ft_itoa.c\
		 ft_memchr.c\
		 ft_memdel.c\
		 ft_memcpy.c\
		 ft_memcmp.c\
		 ft_memccpy.c\
		 ft_memset.c\
		 ft_printlst.c\
		 ft_printtab.c\
		 ft_putchar.c\
		 ft_memmove.c\
		 ft_putchar_fd.c\
		 ft_putendl.c\
		 ft_putendl_fd.c\
		 ft_putstr.c\
		 ft_putstr_fd.c\
		 ft_putnbr.c\
		 ft_putnbr_fd.c\
		 ft_realloc.c\
		 ft_strchr.c\
		 ft_strclr.c\
		 ft_strcat.c\
		 ft_strdel.c\
		 ft_strcmp.c\
		 ft_strcpy.c\
		 ft_striteri.c\
		 ft_strequ.c\
		 ft_striter.c\
		 ft_strdup.c\
		 ft_strlen.c\
		 ft_strjoin.c\
		 ft_strlcat.c\
		 ft_strmap.c\
		 ft_strncmp.c\
		 ft_strmapi.c\
		 ft_strncat.c\
		 ft_strncpy.c\
		 ft_strnew.c\
		 ft_strnequ.c\
		 ft_strrchr.c\
		 ft_strnstr.c\
		 ft_strrange.c\
		 ft_strswap.c\
		 ft_strsub.c\
		 ft_strstr.c\
		 ft_strtrim.c\
		 ft_strsplit.c\
		 ft_toupper.c\
		 ft_tolower.c\
		 ft_itoa_base.c\
		 ft_intlen.c\
		 ft_putwchar.c\
		 ft_count_bytes.c\
		 ft_putwstr.c\
		 ft_strset.c\
		 ft_itoa_base_u.c\
		 ft_strnchr.c\
		 ft_strwlen.c\
		 ft_free_tab.c\
		 get_next_line.c\
		 ft_putc.c \

OBJ = $(SOURCE:.c=.o)

all: $(NAME)

$(NAME):
	@gcc -Wall -Wextra -Werror -c $(SOURCE) $(LIBFTPRINTF_SRC) -I libft.h
	@ar rc $@ $(OBJ) $(LIBFTPRINTF_OBJ)
	@ranlib $@
	@echo "\033[32mCompiled Libft gdufay\033[0m"

clean:
	@rm -f $(OBJ) $(LIBFTPRINTF_OBJ)
	@echo "\033[32mRemove Object Files Of Libft gdufay\033[0m"

fclean: clean
	@rm -f $(NAME);
	@echo "\033[32mRemove Libft gdufay\033[0m"

re: fclean all
