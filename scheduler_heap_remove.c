/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_heap_remove.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 16:13:14 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/07/14 18:40:21 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

static void	swap_requests(t_request *a, t_request *b)
{
	t_request	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

static void	restore_up(t_request_heap *heap, int index, char *scheduler)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (!compare_requests(heap->requests[index],
				heap->requests[parent], scheduler))
			return ;
		swap_requests(&heap->requests[index], &heap->requests[parent]);
		index = parent;
	}
}

static void	restore_down(t_request_heap *heap, int index, char *scheduler)
{
	int	left;
	int	right;
	int	best;

	while (1)
	{
		left = (2 * index) + 1;
		if (left >= heap->size)
			return ;
		right = left + 1;
		best = left;
		if (right < heap->size && compare_requests(heap->requests[right],
				heap->requests[left], scheduler))
			best = right;
		if (!compare_requests(heap->requests[best],
				heap->requests[index], scheduler))
			return ;
		swap_requests(&heap->requests[index], &heap->requests[best]);
		index = best;
	}
}

int	heap_remove_at(t_request_heap *heap, int index,
		t_request *out, char *scheduler)
{
	int	parent;

	if (!heap || !heap->requests || !out || !scheduler)
		return (0);
	if (index < 0 || index >= heap->size)
		return (0);
	*out = heap->requests[index];
	heap->size--;
	if (index == heap->size)
		return (1);
	heap->requests[index] = heap->requests[heap->size];
	parent = (index - 1) / 2;
	if (index > 0 && compare_requests(heap->requests[index],
			heap->requests[parent], scheduler))
		restore_up(heap, index, scheduler);
	else
		restore_down(heap, index, scheduler);
	return (1);
}

int	heap_remove_coder(t_rules *rules, int coder_id)
{
	t_request	removed;
	int			i;

	if (!rules || !rules->scheduler_queue.requests)
		return (0);
	i = 0;
	while (i < rules->scheduler_queue.size)
	{
		if (rules->scheduler_queue.requests[i].coder_id == coder_id)
			return (heap_remove_at(&rules->scheduler_queue, i,
					&removed, rules->scheduler));
		i++;
	}
	return (0);
}
