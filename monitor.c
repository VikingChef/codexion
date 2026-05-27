/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 14:42:33 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/05/27 13:37:20 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

void	*monitor_routine(void *arg)
{
	t_rules	*rules;

	rules = (t_rules *)arg;
	if (!rules)
		return (NULL);
	while (check_all_done(rules) == 0)
		usleep(1000);
	pthread_mutex_lock(&rules->end_mutex);
	rules->simulation_end = 1;
	pthread_mutex_unlock(&rules->end_mutex);
	return (NULL);
}

int	check_all_done(t_rules *rules)
{
	int	i;

	if (!rules)
		return (1);
	i = 0;
	while (i < rules->num_coders)
	{
		if (!coder_is_done(&rules->coders[i]))
			return (0);
		i++;
	}
	return (1);
}

int	simulation_has_ended(t_rules *rules)
{
	int	ended;

	if (!rules)
		return (1);
	pthread_mutex_lock(&rules->end_mutex);
	ended = rules->simulation_end;
	pthread_mutex_unlock(&rules->end_mutex);
	return (ended);
}
