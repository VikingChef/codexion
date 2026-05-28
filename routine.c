/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 12:44:31 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/05/28 15:53:20 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (!coder)
		return (NULL);
	while (!coder_is_done(coder) && !simulation_has_ended(coder->rules))
		coder_cycle(coder);
	return (NULL);
}

void	coder_cycle(t_coder *coder)
{
	if (!coder)
		return ;
	if (do_compile(coder) == 0)
		return ;
	print_status(coder->rules, coder->id, "is debugging");
	smart_sleep(coder->rules, coder->rules->time_to_debug);
	print_status(coder->rules, coder->id, "is refactoring");
	smart_sleep(coder->rules, coder->rules->time_to_refactor);
}

int	coder_is_done(t_coder *coder)
{
	int	done;

	if (!coder)
		return (1);
	if (!coder->rules)
		return (1);
	pthread_mutex_lock(&coder->state_mutex);
	done = coder->compiles_done >= coder->rules->num_compiles_required;
	pthread_mutex_unlock(&coder->state_mutex);
	return (done);
}
