/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_complete.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdufay <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/29 13:17:00 by gdufay            #+#    #+#             */
/*   Updated: 2018/07/10 10:41:56 by gdufay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libedit.h"

t_complete	*create_t_complete(char *name, t_complete *next)
{
	t_complete	*tmp;

	if (!(tmp = (t_complete*)malloc(sizeof(t_complete))))
		return (NULL);
	tmp->name = ft_strdup(name);
	tmp->next = next;
	tmp->head = 0;
	return (tmp);
}

void		add_t_complete(t_complete **head, t_complete **complete,
		char *name)
{
	if (!head || !complete)
		return ;
	if (!*head)
	{
		*head = create_t_complete(name, *head);
		if (*head)
		{
			(*head)->head = 1;
			(*head)->next = *head;
		}
		*complete = *head;
	}
	else
	{
		(*complete)->next = create_t_complete(name, *head);
		*complete = (*complete)->next;
	}
}

void		free_t_complete(t_complete **complete)
{
	t_complete *tmp;

	if (!complete || !*complete)
		return ;
	while (*complete && !(*complete)->head)
		*complete = (*complete)->next;
	if (*complete)
		*complete = (*complete)->next;
	while (*complete && !(*complete)->head)
	{
		tmp = *complete;
		*complete = (*complete)->next;
		ft_strdel(&tmp->name);
		free(tmp);
	}
	ft_strdel(&(*complete)->name);
	free(*complete);
	*complete = NULL;
}

void		t_completejoin(t_complete **head, t_complete *join)
{
	t_complete	*tmp;

	if (!head || !*head || !join)
		return ;
	tmp = *head;
	while (tmp->next && !tmp->next->head)
		tmp = tmp->next;
	tmp->next = join;
	while (join->next && !join->next->head)
		join = join->next;
	join->next->head = 0;
	join->next = *head;
}
