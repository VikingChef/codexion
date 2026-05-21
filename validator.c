/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 11:42:55 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/05/21 13:53:34 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

int	validate_arg_count(int argc)
{
	if (argc != 9)
	{
		printf("Arg_count Error\n");
		return (0);
	}
	else
		return (1);
}

int	validate_arg_numbers(char **argv)
{
	int	i;

	i = 1;
	while (i <= 7)
	{
		if (is_numeric_str(argv[i]) == 0)
		{
			printf("Arg_numbers Error\n");
			return (0);
		}
		i++;
	}
	return (1);
}

int	validate_arg_values(char **argv)
{
	if (!argv)
		return (0);
	if (atoi(argv[1]) == 0)
	{
		printf("Arg_value Error\n");
		return (0);
	}
	return (1);
}

int	validate_args(int argc, char **argv)
{
	if (validate_arg_count(argc) == 0)
		return (0);
	if (validate_arg_numbers(argv) == 0)
		return (0);
	if (validate_arg_values(argv) == 0)
		return (0);
	if (is_valid_scheduler(argv[8]) == 0)
	{
		printf("Scheduler Error\n");
		return (0);
	}
	return (1);
}
