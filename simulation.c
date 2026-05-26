/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 15:11:34 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/05/26 16:20:06 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

int	start_simulation(t_rules *rules)
{
	if (rules == NULL)
		return (0);
	if (create_monitor_thread(rules) == 0)
		return (0);
	if (create_coder_threads(rules) == 0)
		return (0);
	join_coder_threads(rules, rules->num_coders);
	join_monitor_thread(rules);
	return (1);
}

int	create_coder_threads(t_rules *rules)
{
	int	i;

	if (rules == NULL)
		return (0);
	i = 0;
	while (i < rules->num_coders)
	{
		if (pthread_create(&rules->coders[i].thread,
				NULL, coder_routine, &rules->coders[i]) != 0)
		{
			join_coder_threads(rules, i);
			return (0);
		}
		i++;
	}
	return (1);
}

void	join_coder_threads(t_rules *rules, int count)
{
	int	i;

	if (rules == NULL)
		return ;
	i = 0;
	while (i < count)
	{
		pthread_join(rules->coders[i].thread, NULL);
		i++;
	}
}

int	create_monitor_thread(t_rules *rules)
{
	if (rules == NULL)
		return (0);
	if (pthread_create(&rules->monitor_thread,
			NULL, monitor_routine, rules) != 0)
		return (0);
	return (1);
}

void	join_monitor_thread(t_rules *rules)
{
	if (rules == NULL)
		return ;
	pthread_join(rules->monitor_thread, NULL);
}
