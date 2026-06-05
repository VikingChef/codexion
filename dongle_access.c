/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_access.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 09:33:45 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/06/05 12:17:03 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

static int	take_dongle_pair(t_rules *rules, t_coder *coder,
		int left, int right)
{
	t_request	request;

	if (!coder)
		return (0);
	if (!rules)
		return (0);
	if (build_coder_request(coder, &request) == 0)
		return (0);
	lock_two_dongles(rules, left, right);
	if (grant_dongle_pair_request(rules, &request, left, right) == 0)
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
