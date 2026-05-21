/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 14:11:27 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/05/18 06:43:31 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

int	init_one_dongle(t_dongle *dongle, int id)
{
	if (dongle == NULL)
		return (0);
	dongle->id = id;
	dongle->available = 1;
	dongle->cooldown_until = 0;
	if (pthread_mutex_init(&dongle->mutex, NULL) != 0)
		return (0);
	if (pthread_cond_init(&dongle->cond, NULL) != 0)
	{
		pthread_mutex_destroy(&dongle->mutex);
		return (0);
	}
	return (1);
}

int	init_dongles(t_rules *rules)
{
	int	i;

	if (!rules)
		return (0);
	rules->dongles = malloc(rules->num_coders * sizeof(t_dongle));
	if (rules->dongles == NULL)
		return (0);
	i = 0;
	while (i < rules->num_coders)
	{
		if (init_one_dongle(&rules->dongles[i], i) == 0)
		{
			destroy_partial_dongles(rules, i);
			free(rules->dongles);
			rules->dongles = NULL;
			return (0);
		}
		i++;
	}
	return (1);
}

void	destroy_partial_dongles(t_rules *rules, int count)
{
	int	i;

	if (!rules)
		return ;
	if (rules->dongles == NULL)
		return ;
	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&rules->dongles[i].mutex);
		pthread_cond_destroy(&rules->dongles[i].cond);
		i++;
	}
}

void	destroy_dongles(t_rules *rules)
{
	if (!rules)
		return ;
	if (rules->dongles == NULL)
		return ;
	destroy_partial_dongles(rules, rules->num_coders);
	free(rules->dongles);
	rules->dongles = NULL;
}
