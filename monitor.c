/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 14:42:33 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/07/14 14:45:45 by rrasmuss         ###   ########.fr       */
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
	{
		if (check_burnout(rules) == 1)
		{
			pthread_mutex_lock(&rules->end_mutex);
			rules->simulation_end = 1;
			pthread_mutex_unlock(&rules->end_mutex);
			return (NULL);
		}
		usleep(1000);
	}
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

int	check_burnout(t_rules *rules)
{
	int			i;
	long long	deadline;

	if (!rules)
		return (0);
	i = 0;
	while (i < rules->num_coders)
	{
		pthread_mutex_lock(&rules->coders[i].state_mutex);
		deadline = rules->coders[i].last_compile_start;
		pthread_mutex_unlock(&rules->coders[i].state_mutex);
		if (deadline == 0)
			deadline = rules->start_time;
		deadline += rules->time_to_burnout;
		if (get_time_ms() >= deadline)
		{
			print_burnout_status(rules, rules->coders[i].id);
			return (1);
		}
		i++;
	}
	return (0);
}
