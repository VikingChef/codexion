/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logging.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 14:22:53 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/06/05 13:24:48 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

void	print_status(t_rules *rules, int id, char *status)
{
	long long	timestamp;

	if (!rules || !status)
		return ;
	timestamp = get_time_ms() - rules->start_time;
	pthread_mutex_lock(&rules->log_mutex);
	printf("%lld %d %s\n", timestamp, id + 1, status);
	pthread_mutex_unlock(&rules->log_mutex);
}

void	print_error(char *message)
{
	if (message == NULL)
		return ;
	printf("%s\n", message);
}
