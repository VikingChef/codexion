/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_lock.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 12:19:48 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/06/03 12:43:39 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

void	lock_two_dongles(t_rules *rules, int first, int second)
{
	if (!rules || !rules->dongles)
		return ;
	if (first == second)
	{
		pthread_mutex_lock(&rules->dongles[first].mutex);
		return ;
	}
	if (first < second)
	{
		pthread_mutex_lock(&rules->dongles[first].mutex);
		pthread_mutex_lock(&rules->dongles[second].mutex);
	}
	else
	{
		pthread_mutex_lock(&rules->dongles[second].mutex);
		pthread_mutex_lock(&rules->dongles[first].mutex);
	}
}

void	unlock_two_dongles(t_rules *rules, int first, int second)
{
	if (!rules || !rules->dongles)
		return ;
	if (first == second)
	{
		pthread_mutex_unlock(&rules->dongles[first].mutex);
		return ;
	}
	if (first < second)
	{
		pthread_mutex_unlock(&rules->dongles[first].mutex);
		pthread_mutex_unlock(&rules->dongles[second].mutex);
	}
	else
	{
		pthread_mutex_unlock(&rules->dongles[second].mutex);
		pthread_mutex_unlock(&rules->dongles[first].mutex);
	}
}
