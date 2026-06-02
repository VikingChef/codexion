/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 09:44:25 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/06/02 11:51:44 by rrasmuss         ###   ########.fr       */
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

static int	compare_requests(t_request a, t_request b, char *scheduler)
{
	if (scheduler == NULL)
		return (0);
	if (strcmp(scheduler, "fifo") == 0)
	{
		if (a.arrival_time < b.arrival_time)
			return (1);
		if (a.arrival_time == b.arrival_time && a.coder_id < b.coder_id)
			return (1);
		return (0);
	}
	if (strcmp(scheduler, "edf") == 0)
	{
		if (a.deadline < b.deadline)
			return (1);
		if (a.deadline == b.deadline && a.coder_id < b.coder_id)
			return (1);
		return (0);
	}
	return (0);
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
