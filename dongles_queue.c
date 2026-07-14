/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_queue.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 14:50:13 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/07/14 18:36:16 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

static int	request_state(t_rules *rules, t_request request, int *reserved)
{
	int			left;
	int			right;
	int			state;
	long long	now;

	left = request.coder_id;
	right = (left + 1) % rules->num_coders;
	if (reserved[left] || reserved[right])
		return (0);
	lock_two_dongles(rules, left, right);
	now = get_time_ms();
	state = rules->dongles[left].available
		&& rules->dongles[right].available;
	if (state && (rules->dongles[left].cooldown_until > now
			|| rules->dongles[right].cooldown_until > now))
		state = 1;
	else if (state)
		state = 2;
	unlock_two_dongles(rules, left, right);
	if (state)
		reserved[left] = 1;
	if (state)
		reserved[right] = 1;
	return (state);
}

static int	build_ordered(t_rules *rules, t_request *copy,
		t_request *ordered, int size)
{
	t_request_heap	heap;
	int				i;

	heap.requests = copy;
	heap.size = size;
	heap.capacity = size;
	i = 0;
	while (i < size)
	{
		copy[i] = rules->scheduler_queue.requests[i];
		i++;
	}
	i = 0;
	while (i < size)
	{
		if (!heap_pop(&heap, &ordered[i], rules->scheduler))
			return (0);
		i++;
	}
	return (1);
}

static void	grant_request(t_rules *rules, t_request request)
{
	int	left;
	int	right;

	left = request.coder_id;
	right = (left + 1) % rules->num_coders;
	lock_two_dongles(rules, left, right);
	rules->dongles[left].available = 0;
	rules->dongles[right].available = 0;
	mark_coder_granted(rules, left);
	unlock_two_dongles(rules, left, right);
	heap_remove_coder(rules, left);
}

static void	run_scheduler(t_rules *rules)
{
	t_request	*copy;
	t_request	*ordered;
	int			*reserved;
	int			size;
	int			i;

	size = rules->scheduler_queue.size;
	copy = malloc(sizeof(t_request) * size);
	ordered = malloc(sizeof(t_request) * size);
	reserved = calloc(rules->num_coders, sizeof(int));
	if (!copy || !ordered || !reserved)
		return (free(copy), free(ordered), free(reserved));
	if (!build_ordered(rules, copy, ordered, size))
		return (free(copy), free(ordered), free(reserved));
	i = 0;
	while (i < size)
	{
		if (request_state(rules, ordered[i], reserved) == 2)
			grant_request(rules, ordered[i]);
		i++;
	}
	free(copy);
	free(ordered);
	free(reserved);
}

int	grant_dongle_pair_request(t_rules *rules, t_request *request,
		int left, int right)
{
	if (!rules || !request || left == right)
		return (0);
	pthread_mutex_lock(&rules->scheduler_mutex);
	if (!heap_push(&rules->scheduler_queue, *request, rules->scheduler))
		return (pthread_mutex_unlock(&rules->scheduler_mutex), 0);
	pthread_mutex_unlock(&rules->scheduler_mutex);
	while (!simulation_has_ended(rules))
	{
		pthread_mutex_lock(&rules->scheduler_mutex);
		run_scheduler(rules);
		if (collect_coder_grant(rules, request->coder_id))
			return (pthread_mutex_unlock(&rules->scheduler_mutex), 1);
		pthread_mutex_unlock(&rules->scheduler_mutex);
		usleep(500);
	}
	pthread_mutex_lock(&rules->scheduler_mutex);
	heap_remove_coder(rules, request->coder_id);
	pthread_mutex_unlock(&rules->scheduler_mutex);
	return (0);
}
