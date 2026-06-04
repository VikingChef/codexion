/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_access.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 09:33:45 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/06/04 10:59:09 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

static int	can_take_dongle_pair(t_rules *rules, int left, int right)
{
	long long	now;

	if (simulation_has_ended(rules))
		return (0);
	now = get_time_ms();
	if (rules->dongles[left].cooldown_until > now
		|| rules->dongles[right].cooldown_until > now)
		return (0);
	if (rules->dongles[left].available == 0
		|| rules->dongles[right].available == 0)
		return (0);
	return (1);
}

static int	take_dongle_pair(t_rules *rules, t_coder *coder,
		int left, int right)
{
	if (!coder)
		return (0);
	if (!rules)
		return (0);
	lock_two_dongles(rules, left, right);
	if (!can_take_dongle_pair(rules, left, right))
	{
		unlock_two_dongles(rules, left, right);
		return (0);
	}
	rules->dongles[left].available = 0;
	print_status(rules, coder->id, "has taken a dongle");
	rules->dongles[right].available = 0;
	print_status(rules, coder->id, "has taken a dongle");
	unlock_two_dongles(rules, left, right);
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
	return (take_dongle_pair(rules, coder, left, right));
}

void	release_dongles(t_coder *coder)
{
	t_rules		*rules;
	int			left;
	int			right;
	long long	cooldown_until;

	if (!coder)
		return ;
	rules = coder->rules;
	if (!rules)
		return ;
	left = coder->id;
	right = (coder->id + 1) % rules->num_coders;
	cooldown_until = get_time_ms() + rules->dongle_cooldown;
	lock_two_dongles(rules, left, right);
	rules->dongles[right].cooldown_until = cooldown_until;
	rules->dongles[right].available = 1;
	pthread_cond_broadcast(&rules->dongles[right].cond);
	rules->dongles[left].cooldown_until = cooldown_until;
	rules->dongles[left].available = 1;
	pthread_cond_broadcast(&rules->dongles[left].cond);
	unlock_two_dongles(rules, left, right);
}
