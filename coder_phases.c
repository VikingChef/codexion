/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_phases.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 14:57:51 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/06/01 09:34:59 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

int	do_compile(t_coder *coder)
{
	if (!coder)
		return (0);
	if (take_dongles(coder) == 0)
		return (0);
	if (simulation_has_ended(coder->rules))
	{
		release_dongles(coder);
		return (0);
	}
	pthread_mutex_lock(&coder->state_mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->state_mutex);
	print_status(coder->rules, coder->id, "is compiling");
	smart_sleep(coder->rules, coder->rules->time_to_compile);
	if (simulation_has_ended(coder->rules))
	{
		release_dongles(coder);
		return (0);
	}
	pthread_mutex_lock(&coder->state_mutex);
	coder->compiles_done++;
	pthread_mutex_unlock(&coder->state_mutex);
	release_dongles(coder);
	return (1);
}

int	do_debug(t_coder *coder)
{
	if (!coder)
		return (0);
	if (!coder->rules)
		return (0);
	if (simulation_has_ended(coder->rules))
		return (0);
	print_status(coder->rules, coder->id, "is debugging");
	smart_sleep(coder->rules, coder->rules->time_to_debug);
	if (simulation_has_ended(coder->rules))
		return (0);
	return (1);
}

int	do_refactor(t_coder *coder)
{
	if (!coder)
		return (0);
	if (!coder->rules)
		return (0);
	if (simulation_has_ended(coder->rules))
		return (0);
	print_status(coder->rules, coder->id, "is refactoring");
	smart_sleep(coder->rules, coder->rules->time_to_refactor);
	if (simulation_has_ended(coder->rules))
		return (0);
	return (1);
}
