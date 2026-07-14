/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 07:06:46 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/07/14 17:44:48 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

int	init_one_coder(t_coder *coder, int id, t_rules *rules)
{
	if (coder == NULL)
		return (0);
	if (rules == NULL)
		return (0);
	coder->id = id;
	coder->compiles_done = 0;
	coder->last_compile_start = 0;
	coder->granted = 0;
	coder->rules = rules;
	if (pthread_mutex_init(&coder->state_mutex, NULL) != 0)
		return (0);
	return (1);
}

int	init_coders(t_rules *rules)
{
	int	i;

	if (rules == NULL)
		return (0);
	rules->coders = malloc(rules->num_coders * sizeof(t_coder));
	if (rules->coders == NULL)
		return (0);
	i = 0;
	while (i < rules->num_coders)
	{
		if (init_one_coder(&rules->coders[i], i, rules) == 0)
		{
			destroy_partial_coders(rules, i);
			free(rules->coders);
			rules->coders = NULL;
			return (0);
		}
		i++;
	}
	return (1);
}

void	destroy_partial_coders(t_rules *rules, int count)
{
	int	i;

	if (!rules)
		return ;
	if (rules->coders == NULL)
		return ;
	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&rules->coders[i].state_mutex);
		i++;
	}
}

void	destroy_coders(t_rules *rules)
{
	if (!rules)
		return ;
	if (rules->coders == NULL)
		return ;
	destroy_partial_coders(rules, rules->num_coders);
	free(rules->coders);
	rules->coders = NULL;
}
