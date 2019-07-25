/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleroux <pleroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/25 18:15:06 by pleroux           #+#    #+#             */
/*   Updated: 2019/07/25 19:13:32 by pleroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_printf.h>
#include "malloc.h"

void				show_zone(t_zone *zone, t_uint64 *total)
{
	t_alloc			*alloc;

	if (!zone)
		return ;
	alloc = (t_alloc*)(zone + 1);
	while (alloc)
	{
		ft_printf("%x - %x : %d octets\n", \
				alloc + 1, (void*)(alloc + 1) + alloc->length, alloc->length);
		*total += alloc->length;
		alloc = alloc->next;
	}
}

void				show_all_zone(t_zone *zone, char *name, t_uint64 *total)
{
	if (!zone)
		return ;
	while (zone)
	{
		ft_printf("%s : %x\n", name, zone);
		show_zone(zone, total);
	}
}

void				show_alloc_mem(void)
{
	t_uint64		total;

	total = 0;
	show_all_zone(g_mem.tiny, "TINY", &total);
	show_all_zone(g_mem.small, "SMALL", &total);
	show_all_zone(g_mem.large, "LARGE", &total);
	ft_printf("Total : %ld octets\n", total);
}