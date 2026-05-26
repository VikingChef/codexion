/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 16:56:18 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/05/26 09:27:02 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

int	is_numeric_str(char *str)
{
	int	i;

	if (!str || str[0] == 0)
		return (0);
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	is_valid_scheduler(char *str)
{
	if (!str)
		return (0);
	if (strcmp(str, "fifo") == 0)
		return (1);
	else if (strcmp(str, "edf") == 0)
		return (1);
	else
		return (0);
}

long long	get_time_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
		return (0);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	smart_sleep(long long duration)
{
	long long	start;

	start = get_time_ms();
	while (get_time_ms() - start < duration)
		usleep (500);
}
