# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rrasmuss <rrasmuss@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/13 12:52:51 by rrasmuss          #+#    #+#              #
#    Updated: 2026/05/28 15:01:43 by rrasmuss         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = codexion

# List all source files, including bonus ones
SRC = main.c \
	validator.c \
	utils.c \
	init.c \
	cleanup.c \
	dongles.c \
	coders.c \
	logging.c \
	simulation.c \
	routine.c \
	dongle_access.c \
	monitor.c \
	coder_phases.c

# Convert .c files to .o files
OBJ = $(SRC:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

# Build the executable
all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)
 
fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re