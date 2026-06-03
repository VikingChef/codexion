/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 16:36:49 by rrasmuss          #+#    #+#             */
/*   Updated: 2026/06/03 12:45:07 by rrasmuss         ###   ########.fr       */
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

typedef struct s_request
{
	int			coder_id;
	long long	arrival_time;
	long long	deadline;
}	t_request;

typedef struct s_request_heap
{
	t_request	*requests;
	int			size;
	int			capacity;
}	t_request_heap;

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
	t_request_heap	queue;
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
	pthread_mutex_t	end_mutex;
	pthread_t		monitor_thread;
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
int			init_end_mutex(t_rules *rules);
void		cleanup_rules(t_rules *rules);
int			init_dongles(t_rules *rules);
int			init_one_dongle(t_dongle *dongle, int id, int capacity);
void		destroy_dongles(t_rules *rules);
void		destroy_partial_dongles(t_rules *rules, int count);
void		lock_two_dongles(t_rules *rules, int first, int second);
void		unlock_two_dongles(t_rules *rules, int first, int second);
int			take_one_dongle(t_rules *rules, t_coder *coder, int dongle_id);
int			take_dongles(t_coder *coder);
void		release_dongles(t_coder *coder);
int			init_one_coder(t_coder *coder, int id, t_rules *rules);
int			init_coders(t_rules *rules);
void		destroy_partial_coders(t_rules *rules, int count);
void		destroy_coders(t_rules *rules);
int			init_program(t_rules *rules, char **argv);
long long	get_time_ms(void);
void		print_status(t_rules *rules, int id, char *status);
void		print_error(char *message);
int			start_simulation(t_rules *rules);
int			simulation_has_ended(t_rules *rules);
int			create_coder_threads(t_rules *rules);
void		*coder_routine(void *arg);
void		join_coder_threads(t_rules *rules, int count);
int			create_monitor_thread(t_rules *rules);
void		join_monitor_thread(t_rules *rules);
void		coder_cycle(t_coder *coder);
int			coder_is_done(t_coder *coder);
int			do_compile(t_coder *coder);
int			do_debug(t_coder *coder);
int			do_refactor(t_coder *coder);
void		smart_sleep(t_rules *rules, long long duration);
void		*monitor_routine(void *arg);
int			check_all_done(t_rules *rules);
int			check_burnout(t_rules *rules);
int			compare_requests(t_request a, t_request b, char *scheduler);
int			heap_push(t_request_heap *heap, t_request request,
				char *scheduler);
int			heap_peek(t_request_heap *heap, t_request *out);
int			heap_pop(t_request_heap *heap, t_request *out,
				char *scheduler);
int			build_coder_request(t_coder *coder, t_request *out);

#endif