/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 14:22:53 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/07/14 18:50:52 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

static int	logging_is_allowed(t_rules *rules)
{
	int	allowed;

	pthread_mutex_lock(&rules->end_mutex);
	allowed = !rules->simulation_end;
	pthread_mutex_unlock(&rules->end_mutex);
	return (allowed);
}

void	print_status(t_rules *rules, int id, char *status)
{
	long long	timestamp;

	if (!rules || !status)
		return ;
	pthread_mutex_lock(&rules->log_mutex);
	if (!logging_is_allowed(rules))
	{
		pthread_mutex_unlock(&rules->log_mutex);
		return ;
	}
	timestamp = get_time_ms() - rules->start_time;
	printf("%lld %d %s\n", timestamp, id + 1, status);
	pthread_mutex_unlock(&rules->log_mutex);
}

void	print_compile_status(t_rules *rules, int id, int left, int right)
{
	long long	timestamp;

	if (!rules)
		return ;
	pthread_mutex_lock(&rules->log_mutex);
	if (!logging_is_allowed(rules))
	{
		pthread_mutex_unlock(&rules->log_mutex);
		return ;
	}
	timestamp = get_time_ms() - rules->start_time;
	printf("%lld %d has taken a dongle %d\n", timestamp, id + 1,
		left + 1);
	printf("%lld %d has taken a dongle %d\n", timestamp, id + 1,
		right + 1);
	printf("%lld %d is compiling\n", timestamp, id + 1);
	pthread_mutex_unlock(&rules->log_mutex);
}

void	print_burnout_status(t_rules *rules, int id)
{
	long long	timestamp;

	if (!rules)
		return ;
	pthread_mutex_lock(&rules->log_mutex);
	pthread_mutex_lock(&rules->end_mutex);
	if (rules->simulation_end)
	{
		pthread_mutex_unlock(&rules->end_mutex);
		pthread_mutex_unlock(&rules->log_mutex);
		return ;
	}
	rules->simulation_end = 1;
	pthread_mutex_unlock(&rules->end_mutex);
	timestamp = get_time_ms() - rules->start_time;
	printf("%lld %d burned out\n", timestamp, id + 1);
	pthread_mutex_unlock(&rules->log_mutex);
}

void	print_error(char *message)
{
	if (!message)
		return ;
	printf("%s\n", message);
}
