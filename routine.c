/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 12:44:31 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/05/26 09:24:55 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (!coder)
		return (NULL);
	coder_cycle(coder);
	return (NULL);
}

void	coder_cycle(t_coder *coder)
{
	if (!coder)
		return ;
	print_status(coder->rules, coder->id, "is compiling");
	smart_sleep(coder->rules->time_to_compile);
	print_status(coder->rules, coder->id, "is debugging");
	smart_sleep(coder->rules->time_to_debug);
	print_status(coder->rules, coder->id, "is refactoring");
	smart_sleep(coder->rules->time_to_refactor);
}
