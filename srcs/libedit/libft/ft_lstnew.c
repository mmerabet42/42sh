/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/11 15:18:39 by gdufay            #+#    #+#             */
/*   Updated: 2017/11/11 15:42:38 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list		*ft_lstnew(void const *content, size_t content_size)
{
	t_list *tmp;

	if (!(tmp = malloc(sizeof(t_list))))
		return (NULL);
	if (!content)
		tmp->content = NULL;
	else
	{
		tmp->content = ft_memalloc(content_size);
		tmp->content = ft_memmove(tmp->content, content, content_size);
	}
	tmp->content_size = (!content ? 0 : content_size);
	tmp->next = NULL;
	return (tmp);
}
