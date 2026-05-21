/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 16:38:41 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/05/21 13:40:30 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

int	main(int argc, char **argv)
{
	t_rules	rules;

	if (validate_args(argc, argv) == 0)
		return (1);
	if (init_program(&rules, argv) == 0)
		return (1);
	cleanup_rules(&rules);
	return (0);
}
