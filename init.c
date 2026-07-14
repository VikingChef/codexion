/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 11:41:03 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/07/14 16:02:25 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

void	init_rules(t_rules *rules, char **argv)
{
	if (!rules || !argv)
		return ;
	rules->num_coders = atoi(argv[1]);
	rules->time_to_burnout = atoi(argv[2]);
	rules->time_to_compile = atoi(argv[3]);
	rules->time_to_debug = atoi(argv[4]);
	rules->time_to_refactor = atoi(argv[5]);
	rules->num_compiles_required = atoi(argv[6]);
	rules->dongle_cooldown = atoi(argv[7]);
	rules->scheduler = argv[8];
	rules->simulation_end = 0;
	rules->start_time = 0;
	rules->monitor_thread = 0;
	rules->dongles = NULL;
	rules->coders = NULL;
	rules->scheduler_queue.requests = NULL;
	rules->scheduler_queue.size = 0;
	rules->scheduler_queue.capacity = 0;
}

int	init_log_mutex(t_rules *rules)
{
	if (!rules)
		return (0);
	if (pthread_mutex_init(&rules->log_mutex, NULL) != 0)
		return (0);
	return (1);
}

int	init_end_mutex(t_rules *rules)
{
	if (!rules)
		return (0);
	if (pthread_mutex_init(&rules->end_mutex, NULL) != 0)
		return (0);
	return (1);
}

int	init_scheduler(t_rules *rules)
{
	if (!rules)
		return (0);
	rules->scheduler_queue.capacity = rules->num_coders;
	rules->scheduler_queue.size = 0;
	rules->scheduler_queue.requests = malloc(sizeof(t_request)
			* rules->num_coders);
	if (!rules->scheduler_queue.requests)
		return (0);
	if (pthread_mutex_init(&rules->scheduler_mutex, NULL) != 0)
	{
		free(rules->scheduler_queue.requests);
		rules->scheduler_queue.requests = NULL;
		rules->scheduler_queue.capacity = 0;
		return (0);
	}
	return (1);
}

int	init_program(t_rules *rules, char **argv)
{
	if (!rules || !argv)
		return (0);
	init_rules(rules, argv);
	if (init_log_mutex(rules) == 0)
		return (0);
	if (init_end_mutex(rules) == 0)
	{
		pthread_mutex_destroy(&rules->log_mutex);
		return (0);
	}
	if (init_scheduler(rules) == 0)
	{
		destroy_base_mutexes(rules);
		return (0);
	}
	if (init_dongles(rules) == 0 || init_coders(rules) == 0)
	{
		cleanup_rules(rules);
		return (0);
	}
	rules->start_time = get_time_ms();
	return (1);
}
