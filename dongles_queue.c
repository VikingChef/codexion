/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_queue.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 14:50:13 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/06/05 12:15:30 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

int	push_dongle_pair_request(t_rules *rules, t_request request,
		int left, int right)
{
	if (!rules || !rules->dongles || !rules->scheduler)
		return (0);
	if (left == right)
	{
		if (rules->dongles[left].queue.size
			>= rules->dongles[left].queue.capacity)
			return (0);
		return (heap_push(&rules->dongles[left].queue, request,
				rules->scheduler));
	}
	if (rules->dongles[left].queue.size
		>= rules->dongles[left].queue.capacity)
		return (0);
	if (rules->dongles[right].queue.size
		>= rules->dongles[right].queue.capacity)
		return (0);
	if (heap_push(&rules->dongles[left].queue, request,
			rules->scheduler) == 0)
		return (0);
	if (heap_push(&rules->dongles[right].queue, request,
			rules->scheduler) == 0)
		return (0);
	return (1);
}

int	pop_dongle_pair_request(t_rules *rules, t_request *out,
		int left, int right)
{
	t_request	left_request;
	t_request	right_request;

	if (!rules || !rules->dongles || !rules->scheduler || !out)
		return (0);
	if (left == right)
	{
		return (heap_pop(&rules->dongles[left].queue, out,
				rules->scheduler));
	}
	if (heap_peek(&rules->dongles[left].queue, &left_request) == 0)
		return (0);
	if (heap_peek(&rules->dongles[right].queue, &right_request) == 0)
		return (0);
	if (left_request.coder_id != right_request.coder_id)
		return (0);
	if (heap_pop(&rules->dongles[left].queue, out, rules->scheduler) == 0)
		return (0);
	if (heap_pop(&rules->dongles[right].queue,
			&right_request, rules->scheduler) == 0)
		return (0);
	return (1);
}

static int	coder_has_queue_turn(t_rules *rules, int coder_id,
		int left, int right)
{
	t_request	left_request;
	t_request	right_request;

	if (!rules || !rules->dongles)
		return (0);
	if (heap_peek(&rules->dongles[left].queue, &left_request) == 0)
		return (0);
	if (heap_peek(&rules->dongles[right].queue, &right_request) == 0)
		return (0);
	if (left_request.coder_id != coder_id)
		return (0);
	if (right_request.coder_id != coder_id)
		return (0);
	return (1);
}

static int	pair_request_can_be_granted(t_rules *rules,
		t_request *request, int left, int right)
{
	long long	now;

	if (!rules || !rules->dongles || !request)
		return (0);
	now = get_time_ms();
	if (coder_has_queue_turn(rules, request->coder_id, left, right) == 0)
		return (0);
	if (rules->dongles[left].cooldown_until > now
		|| rules->dongles[right].cooldown_until > now)
		return (0);
	if (rules->dongles[left].available == 0
		|| rules->dongles[right].available == 0)
		return (0);
	return (1);
}

int	grant_dongle_pair_request(t_rules *rules, t_request *request,
		int left, int right)
{
	if (!rules || !rules->dongles || !rules->scheduler || !request)
		return (0);
	if (push_dongle_pair_request(rules, *request, left, right) == 0)
		return (0);
	while (pair_request_can_be_granted(rules, request, left, right) == 0)
	{
		if (simulation_has_ended(rules))
			return (0);
		unlock_two_dongles(rules, left, right);
		usleep(500);
		lock_two_dongles(rules, left, right);
	}
	if (pop_dongle_pair_request(rules, request, left, right) == 0)
		return (0);
	return (1);
}
