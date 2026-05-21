/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 16:36:49 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/05/21 14:24:19 by rrasmuss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODERS_H
# define CODERS_H

# include <pthread.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

/*
** the dongle
*/

typedef struct s_dongle
{
	int				id;
	int				available;
	long long		cooldown_until;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
}	t_dongle;

typedef struct s_coder	t_coder;
/*
** The Rules of the Simulation
*/

typedef struct s_rules
{
	int				num_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				num_compiles_required;
	int				dongle_cooldown;
	char			*scheduler;
	long long		start_time;
	int				simulation_end;
	pthread_mutex_t	log_mutex;
	t_dongle		*dongles;
	t_coder			*coders;
}	t_rules;

/*
** The Individual Coder
*/

typedef struct s_coder
{
	int				id;
	int				compiles_done;
	long long		last_compile_start;
	pthread_t		thread;
	pthread_mutex_t	state_mutex;
	t_rules			*rules;
}	t_coder;

int			is_numeric_str(char *str);
int			is_valid_scheduler(char *str);
int			validate_arg_count(int argc);
int			validate_arg_numbers(char **argv);
int			validate_arg_values(char **argv);
int			validate_args(int argc, char **argv);
void		init_rules(t_rules *rules, char **argv);
int			init_log_mutex(t_rules *rules);
void		cleanup_rules(t_rules *rules);
int			init_dongles(t_rules *rules);
int			init_one_dongle(t_dongle *dongle, int id);
void		destroy_dongles(t_rules *rules);
void		destroy_partial_dongles(t_rules *rules, int count);
int			init_one_coder(t_coder *coder, int id, t_rules *rules);
int			init_coders(t_rules *rules);
void		destroy_partial_coders(t_rules *rules, int count);
void		destroy_coders(t_rules *rules);
int			init_program(t_rules *rules, char **argv);
long long	get_time_ms(void);
void		print_status(t_rules *rules, int id, char *status);

#endif