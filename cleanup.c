/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 13:16:12 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/05/21 13:35:30 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

void	cleanup_rules(t_rules *rules)
{
	if (rules == NULL)
		return ;
	destroy_coders(rules);
	destroy_dongles(rules);
	pthread_mutex_destroy(&rules->log_mutex);
}
