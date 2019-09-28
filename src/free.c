/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleroux <pleroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/21 15:09:38 by pleroux           #+#    #+#             */
/*   Updated: 2019/09/28 23:02:21 by pleroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <libft.h>
#include <errno.h>
#include "malloc.h"

int				find_and_delete_alloc(t_alloc *l, t_alloc *find, int *total)
{
	t_alloc		*previous;
	int			delete;

	if (l == NULL)
		return (FALSE);
	previous = NULL;
	delete = FALSE;
	while (l)
	{
		if (l == find)
		{
			if (previous != NULL)
			{
				previous->next = l->next;
				ft_bzero((void*)find, sizeof(t_alloc) + find->length);
				delete = TRUE;
			}
		}
		else
			*total += l->length;
		previous = l;
		l = l->next;
	}
	return (delete);
}

void			delete_zone(t_zone **head, t_zone *zone)
{
	t_zone		*l;
	t_zone		*previous;

	if (!head && !*head)
		return ;
	l = *head;
	previous = NULL;
	while (l)
	{
		if (l == zone)
		{
			if (previous != NULL)
			{
				previous->next = l->next;
			}
			else
				*head = l->next;
			// printf("FREE: zone %p : %lu\n", (void*)l, l->length + sizeof(t_zone));
			if (munmap((void*)l, l->length + sizeof(t_zone)) < 0)
			{
				// printf("ERROR to free %lu bytes of memory\n",
				// 		l->length + sizeof(t_zone));
			}
			// printf("%s\n", strerror(errno));
			return ;
		}
		previous = l;
		l = l->next;
	}
}

int				find_and_delete_zone(t_zone **head_zone, t_alloc *find)
{
	int			total;
	t_zone		*zone;

	zone = *head_zone;
	while (zone)
	{
		total = 0;
		if (find_and_delete_alloc((t_alloc*)(zone + 1), find, &total))
		{
			// printf("FREE: total %d\n", total);
			if (total == 0)
				delete_zone(head_zone, zone);
			return (TRUE);
		}
		zone = zone->next;
	}
	return (FALSE);
}

void			free(void *ptr)
{
	/*
	ft_putstr("free ");
	ft_putnbr((int)ptr);
	ft_putchar('\n');*/
	t_alloc		*find;

	if (ptr != NULL)
	{
		find = (t_alloc*)(ptr - sizeof(t_alloc));
		if (find_and_delete_zone(&g_mem.large, find))
		{
			// printf("large\n");
			return ;
		}
		else if (find_and_delete_zone(&g_mem.small, find))
		{
			// printf("small\n");
			return ;
		}
		else if (find_and_delete_zone(&g_mem.tiny, find))
		{
			// printf("tiny\n");
			return ;
		}
		// printf("FREE: NOT FOUND\n");
		errno = FREE_ERROR;
	}
	//	write(1, "FREE OUT\n", strlen("FREE_OUT\n"));
}
