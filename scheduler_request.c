/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_request.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 08:44:47 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/06/03 11:17:31 by rrasmuss         ###   ########.fr       */
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
