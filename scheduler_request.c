/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_request.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 08:44:47 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/07/14 18:38:56 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

int	build_coder_request(t_coder *coder, t_request *out)
{
	long long	last_compile_start;

	if (!coder || !out || !coder->rules)
		return (0);
	out->coder_id = coder->id;
	out->arrival_time = get_time_ms();
	pthread_mutex_lock(&coder->state_mutex);
	last_compile_start = coder->last_compile_start;
	pthread_mutex_unlock(&coder->state_mutex);
	if (last_compile_start == 0)
		out->deadline = coder->rules->start_time
			+ coder->rules->time_to_burnout;
	else
		out->deadline = last_compile_start
			+ coder->rules->time_to_burnout;
	return (1);
}

void	mark_coder_granted(t_rules *rules, int coder_id)
{
	if (!rules || coder_id < 0 || coder_id >= rules->num_coders)
		return ;
	pthread_mutex_lock(&rules->coders[coder_id].state_mutex);
	rules->coders[coder_id].last_compile_start = get_time_ms();
	rules->coders[coder_id].granted = 1;
	pthread_mutex_unlock(&rules->coders[coder_id].state_mutex);
}

int	collect_coder_grant(t_rules *rules, int coder_id)
{
	int	granted;

	if (!rules || coder_id < 0 || coder_id >= rules->num_coders)
		return (0);
	pthread_mutex_lock(&rules->coders[coder_id].state_mutex);
	granted = rules->coders[coder_id].granted;
	if (granted)
		rules->coders[coder_id].granted = 0;
	pthread_mutex_unlock(&rules->coders[coder_id].state_mutex);
	return (granted);
}
