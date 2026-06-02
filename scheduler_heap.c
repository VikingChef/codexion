/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_heap.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 09:44:25 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/06/02 15:25:49 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

static void	heap_swap(t_request *a, t_request *b)
{
	t_request	temp;

	if (a == NULL)
		return ;
	if (b == NULL)
		return ;
	temp = *a;
	*a = *b;
	*b = temp;
}

static void	restore_heap_down(t_request_heap *heap, char *scheduler)
{
	int	i;
	int	left;
	int	right;
	int	best;

	if (heap == NULL || !heap->requests || !scheduler)
		return ;
	i = 0;
	while (1)
	{
		left = 2 * i + 1;
		if (left >= heap->size)
			return ;
		right = left + 1;
		best = left;
		if (right < heap->size
			&& compare_requests(heap->requests[right],
				heap->requests[best], scheduler))
			best = right;
		if (!compare_requests(heap->requests[best],
				heap->requests[i], scheduler))
			return ;
		heap_swap(&heap->requests[i], &heap->requests[best]);
		i = best;
	}
}

int	heap_push(t_request_heap *heap, t_request request,
		char *scheduler)
{
	int	i;
	int	parent;

	if (heap == NULL || !heap->requests || !scheduler)
		return (0);
	if (heap->size >= heap->capacity)
		return (0);
	heap->requests[heap->size] = request;
	i = heap->size;
	heap->size++;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (compare_requests(heap->requests[i],
				heap->requests[parent], scheduler))
		{
			heap_swap(&heap->requests[i], &heap->requests[parent]);
			i = parent;
		}
		else
			return (1);
	}
	return (1);
}

int	heap_peek(t_request_heap *heap, t_request *out)
{
	if (heap == NULL || !heap->requests || !out)
		return (0);
	if (heap->size <= 0)
		return (0);
	*out = heap->requests[0];
	return (1);
}

int	heap_pop(t_request_heap *heap, t_request *out,
		char *scheduler)
{
	if (heap == NULL || !heap->requests || !out || !scheduler)
		return (0);
	if (heap->size <= 0)
		return (0);
	*out = heap->requests[0];
	heap->size--;
	if (heap->size == 0)
		return (1);
	heap->requests[0] = heap->requests[heap->size];
	restore_heap_down(heap, scheduler);
	return (1);
}
