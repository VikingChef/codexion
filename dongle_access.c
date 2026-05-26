/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_access.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 09:33:45 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/05/26 11:50:02 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

void	take_one_dongle(t_rules *rules, t_coder *coder, int dongle_id)
{
	if (!coder)
		return ;
	if (!rules)
		return ;
	pthread_mutex_lock(&rules->dongles[dongle_id].mutex);
	print_status(rules, coder->id, "has taken a dongle");
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
	{
		take_one_dongle(rules, coder, left);
		take_one_dongle(rules, coder, right);
	}
	else
	{
		take_one_dongle(rules, coder, right);
		take_one_dongle(rules, coder, left);
	}
	return (1);
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
