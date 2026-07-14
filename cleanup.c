/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 13:16:12 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/07/14 16:03:36 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

void	cleanup_rules(t_rules *rules)
{
	if (!rules)
		return ;
	destroy_coders(rules);
	destroy_dongles(rules);
	pthread_mutex_destroy(&rules->scheduler_mutex);
	free(rules->scheduler_queue.requests);
	rules->scheduler_queue.requests = NULL;
	rules->scheduler_queue.size = 0;
	rules->scheduler_queue.capacity = 0;
	destroy_base_mutexes(rules);
}

void	destroy_base_mutexes(t_rules *rules)
{
	if (!rules)
		return ;
	pthread_mutex_destroy(&rules->end_mutex);
	pthread_mutex_destroy(&rules->log_mutex);
}
