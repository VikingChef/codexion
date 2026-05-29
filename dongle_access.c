/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_access.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 09:33:45 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/05/29 14:21:16 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

int	take_one_dongle(t_rules *rules, t_coder *coder, int dongle_id)
{
	if (!coder)
		return (0);
	if (!rules)
		return (0);
	pthread_mutex_lock(&rules->dongles[dongle_id].mutex);
	if (simulation_has_ended(rules))
	{
		pthread_mutex_unlock(&rules->dongles[dongle_id].mutex);
		return (0);
	}
	print_status(rules, coder->id, "has taken a dongle");
	return (1);
}

static int	take_ordered_dongles(t_rules *rules, t_coder *coder,
		int first, int second)
{
	if (!coder)
		return (0);
	if (!rules)
		return (0);
	if (!take_one_dongle(rules, coder, first))
		return (0);
	if (!take_one_dongle(rules, coder, second))
	{
		pthread_mutex_unlock(&rules->dongles[first].mutex);
		return (0);
	}
	return (1);
}

int	take_dongles(t_coder *coder)
{
	t_rules		*rules;
	int			left;
	int			right;

	if (!coder)
		return (0);
	rules = coder->rules;
	if (!rules)
		return (0);
	left = coder->id;
	right = (coder->id + 1) % rules->num_coders;
	if (coder->id % 2 == 0)
		return (take_ordered_dongles(rules, coder, left, right));
	return (take_ordered_dongles(rules, coder, right, left));
}

void	release_dongles(t_coder *coder)
{
	t_rules		*rules;
	int			left;
	int			right;

	if (!coder)
		return ;
	rules = coder->rules;
	if (!rules)
		return ;
	left = coder->id;
	right = (coder->id + 1) % rules->num_coders;
	pthread_mutex_unlock(&rules->dongles[right].mutex);
	pthread_mutex_unlock(&rules->dongles[left].mutex);
}
