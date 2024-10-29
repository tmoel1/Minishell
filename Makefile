# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmoeller <tmoeller@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/22 11:11:02 by tmoeller          #+#    #+#              #
#    Updated: 2024/10/22 12:56:13 by tmoeller         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# --------------------------------- VARIABLES --------------------------------- #

NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
RM			= rm -f

# Define the objects directory
OBJDIR		= objects

# Source files
SRCS		= Builtins/builtin_commands_1.c \
			Builtins/builtin_commands_2.c \
			Builtin/builtin_commands_helpers_1.c \
			Builtin/builtin_commands_helpers_2.c \
			Environment/environment_list_management.c \
			Environment/environment_array_conversion.c \
			Environment/environment_variables.c \
			Execution/command_execution.c \
			Execution/external_command_execution.c \
			Execution/external_command_helpers.c \
			Main/main.c \
			Main/signal_handling.c \
			Parsing/input_processing.c \
			Parsing/variable_expansion.c \
			Pipes/pipeline_execution_helpers.c \
			Pipes/pipeline_execution.c \
			Pipes/pipeline_splitting_helpers.c \
			Pipes/pipeline_splitting.c \
			Redirects/redirection_handling.c \
			Redirects/redirection_helpers.c \
			Tokenization/tokenization_1.c \
			Tokenization/tokenization_2.c \
			Tokenization/tokenization_helpers.c \
			Utilities/split_and_helpers.c \
			Utilities/utility_functions_1.c \
			Utilities/utility_functions_2.c \
			Utilities/utility_functions_3.c \
			Utilities/utility_functions_4.c \
			Utilities/utility_functions_5.c

# Object files placed in the objects directory
OBJS		= $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.c=.o)))

# --------------------------------- RULES ------------------------------------- #

# Pattern rules for compiling .c files from various subdirectories to .o files in the objects directory
$(OBJDIR)/%.o: Builtins/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -I. -c $< -o $@

$(OBJDIR)/%.o: Environment/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -I. -c $< -o $@

$(OBJDIR)/%.o: Execution/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -I. -c $< -o $@

$(OBJDIR)/%.o: Main/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -I. -c $< -o $@

$(OBJDIR)/%.o: Parsing/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -I. -c $< -o $@

$(OBJDIR)/%.o: Pipes/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -I. -c $< -o $@

$(OBJDIR)/%.o: Redirects/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -I. -c $< -o $@

$(OBJDIR)/%.o: Tokenization/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -I. -c $< -o $@

$(OBJDIR)/%.o: Utilities/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -I. -c $< -o $@

# Default target
all: $(NAME)

# Linking the executable
$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -lreadline
	@echo "✅ Minishell compiled successfully."

# Clean up object files
clean:
	$(RM) $(OBJS)

# Clean up object files and executable
fclean: clean
	$(RM) $(NAME)

# Rebuild the project from scratch
re: fclean all

# Phony targets to prevent conflicts with files named 'all', 'clean', etc.
.PHONY: all clean fclean re