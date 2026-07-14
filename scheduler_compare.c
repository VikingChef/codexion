/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_compare.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 12:43:01 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/07/14 16:55:08 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

int	compare_requests(t_request a, t_request b, char *scheduler)
{
	if (scheduler == NULL)
		return (0);
	if (strcmp(scheduler, "fifo") == 0)
	{
		if (a.arrival_time < b.arrival_time)
			return (1);
		if (a.arrival_time == b.arrival_time && a.coder_id < b.coder_id)
			return (1);
		return (0);
	}
	if (strcmp(scheduler, "edf") == 0)
	{
		if (a.deadline < b.deadline)
			return (1);
		if (a.deadline == b.deadline
			&& a.arrival_time < b.arrival_time)
			return (1);
		if (a.deadline == b.deadline
			&& a.arrival_time == b.arrival_time
			&& a.coder_id < b.coder_id)
			return (1);
		return (0);
	}
	return (0);
}
