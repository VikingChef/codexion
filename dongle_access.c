/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_access.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 09:33:45 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/07/14 18:48:15 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

static int	take_dongle_pair(t_rules *rules, t_coder *coder,
		int left, int right)
{
	t_request	request;

	if (!rules || !coder)
		return (0);
	if (!build_coder_request(coder, &request))
		return (0);
	if (!grant_dongle_pair_request(rules, &request, left, right))
		return (0);
	print_compile_status(rules, coder->id, left, right);
	return (1);
}

int	take_dongles(t_coder *coder)
{
	t_rules	*rules;
	int		left;
	int		right;

	if (!coder || !coder->rules)
		return (0);
	rules = coder->rules;
	if (rules->num_coders == 1)
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

	if (!coder || !coder->rules)
		return ;
	rules = coder->rules;
	left = coder->id;
	right = (coder->id + 1) % rules->num_coders;
	lock_two_dongles(rules, left, right);
	cooldown_until = get_time_ms() + rules->dongle_cooldown;
	rules->dongles[left].cooldown_until = cooldown_until;
	rules->dongles[right].cooldown_until = cooldown_until;
	rules->dongles[left].available = 1;
	rules->dongles[right].available = 1;
	pthread_cond_broadcast(&rules->dongles[left].cond);
	pthread_cond_broadcast(&rules->dongles[right].cond);
	unlock_two_dongles(rules, left, right);
}
