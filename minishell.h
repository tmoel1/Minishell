/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoeller <tmoeller@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 15:31:01 by tmoeller          #+#    #+#             */
/*   Updated: 2024/10/21 15:45:42 by tmoeller         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include <limits.h>
# include <ctype.h>
# include <stdarg.h>

# define MAX_TOKENS 1000
# define MAX_CMD_LEN 1024
# define PATH_MAX 4096

// Global variable to indicate received signals
extern volatile sig_atomic_t	g_received_signal;

// Environment variable linked list
typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

// Struct to hold variables for pipeline execution
typedef struct s_pipeline_vars
{
	int	i;
	int	cmd_index;
	int	start;
	int	in_s_quote;
	int	in_d_quote;
}	t_pipeline_vars;

// Struct to hold variables for tokenization
typedef struct s_tokenize_vars
{
	int		i;
	int		token_index;
	char	token[MAX_CMD_LEN];
	int		vars[4];
}	t_tokenize_vars;

//andy
/* ************************************************************************** */
/*                               Main Functions                               */
/* ************************************************************************** */

int		main(int argc, char **argv, char **envp);
void	main_loop(t_env **env_list);
void	process_input_line(t_env **env_list, char *input);

/*
** The "Main Functions" section serves as the entry point and core loop of the
** shell program. The `main` function initializes the shell environment,
** loads environment variables into a linked list, and starts the main loop.
** The `main_loop` function continuously prompts the user for input using
** `readline`, adds non-empty inputs to the history, and processes each input
** line. The `process_input_line` function checks if the input contains a pipe
** symbol and decides whether to execute it as a pipeline or a single command.
** These functions work together to provide a responsive and interactive shell
** experience, maintaining the program's state and coordinating the flow of
** execution based on user input.
*/

/* ************************************************************************** */
/*                              Signal Handling                               */
/* ************************************************************************** */

void	handle_signal(int sig);
void	initialize_shell(void);

/*
** The "Signal Handling" section manages the shell's response to system signals.
** The `initialize_shell` function sets up custom handlers for signals like
** SIGINT (Ctrl+C) and ignores SIGQUIT (Ctrl+\). The `handle_signal` function
** defines how the shell reacts when a SIGINT is received: it writes a new line,
** resets the prompt, and updates the global `g_received_signal` variable. This
** allows the shell to gracefully handle interruptions without terminating,
** ensuring a stable user experience even when unexpected signals occur.
*/

/* ************************************************************************** */
/*                          Input Processing                                  */
/* ************************************************************************** */

char	*expand_variables(t_env *env_list, char *input);
void	process_input(t_env *env_list, char *input, char *result);
void	process_input_loop(t_env *env_list, char *input, char *result,
			int *vars);
void	process_single_quote(char *input, char *result, int *vars,
			int quote_index);
void	process_double_quote(char *input, char *result, int *vars,
			int quote_index);

/*
** The "Input Processing" section handles the initial manipulation of user input,
** particularly variable expansion and quote handling. The `expand_variables`
** func allocates memory for the processed input and initiates the processing.
** The `process_input` function sets up variables and calls `process_input_loop`,
** which iterates over each character of the input. It carefully manages the
** state of single and double quotes, ensuring that variables are expanded
** correctly only when appropriate. The `process_single_quote` and
** `process_double_quote` functions toggle the quote state and append characters
** to the result. Together, these functions sanitize and prepare the user input
** for further tokenization and execution, respecting shell parsing rules.
*/

/* ************************************************************************** */
/*                          Variable Expansion                                */
/* ************************************************************************** */

void	process_variable_expansion(t_env *env_list, char *input, char *result,
			int *vars);
void	process_other_character(char *input, char *result, int *vars);
char	*get_variable_value(t_env *env_list, char *var_name);
void	handle_variable_char(char *input, int *i, char *var_name, int *k);
void	ft_strappend(char *dest, const char *src, int *j, int max_len);

/*
** This section focuses on expanding environment variables within the user input.
** The `process_variable_expansion` function handles cases where a `$` symbol is
** encountered, determining whether to replace it with an environment variable
** value or a special parameter like `$?`. The `get_variable_value` function
** retrieves the value of a given variable from the environment linked list.
** `handle_variable_char` parses the variable name from the input, and
** `ft_strappend` appends strings to the result with bounds checking.
** `process_other_character` handles characters that are not part of a variable
** expansion. Collectively, these functions ensure that all variables in the
** input are correctly expanded, enabling dynamic command execution based on the
** current environment.
*/

/* ************************************************************************** */
/*                          Tokenization                                      */
/* ************************************************************************** */

void	tokenize_input(t_env *env_list, char *input, char **tokens);
void	initialize_tokenize_vars(t_tokenize_vars *v);
void	tokenize_input_loop(t_env *env_list, char *input,
			char **tokens, t_tokenize_vars *v);
void	handle_too_many_tokens(char **tokens, int token_index);

/* ************************************************************************** */
/*                          Tokenization_2                                    */
/* ************************************************************************** */

void	process_regular_token(t_env *env_list, char *input, char *token,
			int *vars);
void	process_token_loop(t_env *env_list, char *input, char *token,
			int *vars);
void	expand_variable_in_token(t_env *env_list, char *input, char *token,
			int *vars);
void	handle_special_token(char *input, int *i, char **tokens,
			int *token_index);

/*
** The "Tokenization" section splits the processed input into meaningful tokens
** for execution. The `tokenize_input` function orchestrates this by iterating
** over the input and identifying tokens. It distinguishes between regular tokens
** and special sequences (like redirection symbols or pipes). The
** `process_regular_token` and `process_token_loop` functions handle the
** extraction of regular tokens, respecting quotes and variable expansions within
** them. `expand_variable_in_token` deals specifically with variable expansions
** encountered inside tokens. `handle_special_token` recognizes and processes
** special shell operators. This tokenization process is crucial for correctly
** interpreting the user's commands and preparing them for execution.
*/

/* ************************************************************************** */
/*                          Tokenization Helpers                              */
/* ************************************************************************** */

void	add_token_to_list(char **tokens, int *token_index, char *token);
void	skip_whitespace(char *input, int *i);
int		is_special_sequence(char c);
void	ft_strappend_char(char *dest, char c, int *j, int max_len);
void	free_tokens(char **tokens);

/*
** These helper functions support the tokenization process by handling common
** tasks. `add_token_to_list` adds a new token to the array of tokens.
** `skip_whitespace` moves the index past any whitespace characters.
** `is_special_sequence` checks if a character is a special shell operator,
** aiding in token differentiation. `ft_strappend_char` safely appends a single
** character to a string, ensuring it doesn't exceed the maximum length.
** `free_tokens` deallocates memory used by the tokens array. Together, these
** utilities streamline the tokenization logic, making the main functions more
** readable and maintainable.
*/

/* ************************************************************************** */
/*                          Command Execution                                 */
/* ************************************************************************** */

void	execute_command(t_env **env_list, char *input);
void	process_command(t_env **env_list, char **args);
void	execute_builtin(t_env **env_list, char **args);
int		is_builtin(char *cmd);
void	execute_specific_builtin(t_env **env_list, char **args);

/*
** This section handles the execution of commands parsed from user input.
** `execute_command` tokenizes the input and initiates command processing.
** `process_command` determines if the command is a shell builtin or an external
** program. If it's a builtin, `execute_builtin` is called, which also handles
** any redirections before executing the command. `is_builtin` checks if the
** command matches known builtins. `execute_specific_builtin` calls the
** appropriate function for the recognized builtin command. This structured
** approach allows the shell to efficiently handle both built-in and external
** commands, maintaining modularity and ease of extension.
*/

/* ************************************************************************** */
/*                          External Command Execution                        */
/* ************************************************************************** */

void	execute_external_command(t_env **env_list, char **args);
void	execute_child_process_main(t_env **env_list, char **args);
void	handle_child_exit_status(int status);
void	execute_external(t_env *env_list, char **args, char **envp);
void	try_execute_direct(char **args, char **envp);

/*
** For commands that are not builtins, this section manages their execution.
** `execute_external_command` forks a new process to run the external command.
** In the child process, `execute_child_process_main` sets up the environment and
** executes the command. `handle_child_exit_status` updates the shell's state
** based on how the child process terminated. `execute_external` attempts to find
** and run the command, first by trying direct execution if the command contains
** a slash, and then by searching through the dir specified in the shell's
** own PATH environment variable from `env_list`. `try_execute_direct` handles
** cases where the command might be a path to an executable file. This approach
** ensures that external commands are executed in a way that respects the shell's
** environment, including any changes to the PATH variable made within the shell.
*/

/* ************************************************************************** */
/*                          External Command Helpers                          */
/* ************************************************************************** */

void	execute_non_builtin_command(t_env **env_list, char **args);
char	**get_path_dirs(t_env *env_list);
void	search_and_execute(char **args, char **envp, char **path_dirs);
void	command_not_found(char **args, char **path_dirs);
void	free_split(char **strs);

/*
** These helper functions support the execution of external commands.
** `execute_non_builtin_command` prepares and executes the command with the
** shell's environment. `get_path_dirs` retrieves the directories listed in the
** shell's own PATH environment variable from `env_list`, ensuring that any
** changes to PATH within the shell are respected. `search_and_execute` looks for
** the executable in these directories and runs it if found. If the command isn't
** found, `command_not_found` outputs an error message and exits. `free_split`
** cleans up memory allocated for arrays of strings. These utilities collectively
** manage the complexities of locating and executing external programs while
** respecting the shell's environment.
*/

/* ************************************************************************** */
/*                          Built-in Commands_1                               */
/* ************************************************************************** */

void	execute_echo(char **args);
void	execute_cd(t_env **env_list, char **args);
void	execute_pwd(void);
void	execute_env(t_env *env_list);
void	execute_exit(t_env *env_list, char **args);

/*
** This section implements several shell builtin commands. `execute_echo`
** replicates the behavior of the echo command, supporting the `-n` option to
** suppress the trailing newline. `execute_cd` changes the current working
** directory and updates environment variables accordingly. `execute_pwd` prints
** the current working directory. `execute_env` displays all environment
** variables. `execute_exit` exits the shell, optionally with a specified exit
** status. These functions allow the shell to perform essential operations
** without invoking external programs, improving efficiency and control over the
** shell environment.
*/

/* ************************************************************************** */
/*                          Built-in Commands_2                               */
/* ************************************************************************** */

void	execute_unset(t_env **env_list, char **args);
void	execute_export(t_env **env_list, char **args);

/*
** Continuing with builtin commands, this section includes `execute_unset` and
** `execute_export`. `execute_unset` removes environment variables from the
** shell's environment, while `execute_export` adds new variables or updates
** existing ones. If called without arguments, `execute_export` lists all
** exported variables. These commands are essential for managing the shell's
** environment and influence how processes spawned from the shell will behave.
*/

/* ************************************************************************** */
/*                          Built-in Command Helpers_1                        */
/* ************************************************************************** */

char	*get_cd_path(t_env **env_list, char **args, char *prev_dir);
void	update_pwd(t_env **env_list, char *cwd, char *prev_dir);
void	change_directory(t_env **env_list, char *path, char *cwd,
			char *prev_dir);
void	validate_exit_arg(char *arg, t_env *env_list);
void	export_variable(t_env **env_list, char *arg);

/*
** These helper functions support the builtin commands. `get_cd_path` determines
** the target directory for the `cd` command, handling cases like `cd -` and
** defaulting to the HOME directory. `update_pwd` updates the PWD and OLDPWD
** environment variables after a directory change. `change_directory` performs
** the actual directory change and updates the environment if successful.
** `validate_exit_arg` ensures that the argument provided to `exit` is valid,
** preventing unintended behavior. `export_variable` parses and sets environment
** variables for the `export` command. These functions encapsulate common tasks,
** making the main builtin command functions cleaner and more focused.
*/

/* ************************************************************************** */
/*                          Built-in Command Helpers_2                        */
/* ************************************************************************** */

void	print_exported_variables(t_env *env_list);
void	restore_std_fds(int saved_stdin, int saved_stdout);

/*
** This section provides additional helper functions for builtin commands.
** `print_exported_variables` lists all exported environment variables in a
** formatted manner, used when `export` is called without arguments.
** `restore_std_fds` restores the original standard input and output file
** descriptors after command execution, especially important if redirects were
** applied. These utilities ensure that the shell's state remains consistent and
** that builtins behave correctly in various contexts.
*/

/* ************************************************************************** */
/*                          Redirection Handling                              */
/* ************************************************************************** */

void	handle_redirections(char **args);
void	handle_input_redirection(char **args, int *i);
void	handle_output_redirection(char **args, int *i);
void	handle_append_redirection(char **args, int *i);
void	handle_heredoc_redirection(char **args, int *i);

/*
** The "Redirection Handling" section manages input and output redirections.
** `handle_redirections` scans arguments for redirection operators and
** dispatches to the correct handler. `handle_input_redirection` redirects
** input from a file. `handle_output_redirection` redirects output to a file,
** overwriting it. `handle_append_redirection` appends output to a file.
** `handle_heredoc_redirection` implements heredoc by reading input until a
** delimiter is reached. These functions modify file descriptors, allowing
** commands to read from or write to the specified files, enabling shell
** features like input/output redirection and here-documents.
*/

/* ************************************************************************** */
/*                          Redirection Helpers                               */
/* ************************************************************************** */

void	error_missing_argument(void);
void	remove_redirection_tokens(char **args, int i);
void	read_heredoc(char *delimiter, int write_fd);
void	update_quote_status(char c, int *in_s_quote, int *in_d_quote);

/*
** Supporting the redirection handling, these helper functions perform common
** tasks. `error_missing_argument` outputs an error when a redirection operator
** is missing its operand. `remove_redirection_tokens` cleans up the arguments
** array after handling a redirection. `read_heredoc` reads user input for the
** heredoc feature until the delimiter is matched. `update_quote_status` keeps
** track of whether the parser is inside single or double quotes, which is
** important for correct parsing. These utilities enhance the robustness and
** correctness of command parsing and execution, especially when dealing with
** complex input involving quotes and redirections.
*/

/* ************************************************************************** */
/*                          Pipeline Splitting                                */
/* ************************************************************************** */

int		split_pipeline(char *cmd_line, char **commands);
void	initialize_pipeline_vars(t_pipeline_vars *vars);
int		finalize_pipeline(char *cmd_line, char **commands,
			t_pipeline_vars *vars);
int		parse_pipeline(char *cmd_line, char **commands, t_pipeline_vars *vars);

/*
** The "Pipeline Splitting" section is responsible for breaking down a command
** line containing pipes into individual commands. `split_pipeline` orchestrates
** this process by initializing variables and parsing the command line.
** `initialize_pipeline_vars` sets up the initial state for parsing.
** `parse_pipeline` iterates over the command line, respecting quotes, and
** identifies pipe characters that are not within quotes. `finalize_pipeline`
** handles the last command after the loop completes. These functions ensure that
** pipelines are correctly split into commands that can be individually executed
** and connected via pipes.
*/

/* ************************************************************************** */
/*                          Pipeline Splitting Helpers                        */
/* ************************************************************************** */

int		process_pipeline_token(char *cmd_line, char **commands,
			t_pipeline_vars *vars);
int		allocate_command(char **commands, int *cmd_index, char *command);
int		check_max_commands(int cmd_index);
int		free_commands(char **commands, int cmd_index);

/*
** These helper functions assist in the pipeline splitting process.
** `process_pipeline_token` handles each command segment between pipes.
** `allocate_command` allocates memory for a command and adds it to the list.
** `check_max_commands` ensures that the number of commands does not exceed the
** predefined limit. `free_commands` cleans up allocated memory in case of an
** error during parsing. These functions contribute to robust error handling and
** efficient memory management during pipeline parsing.
*/

/* ************************************************************************** */
/*                          Pipeline Execution                                */
/* ************************************************************************** */

void	execute_pipeline(t_env **env_list, char *cmd_line);
void	execute_command_loop(t_env **env_list, char **commands,
			int num_commands);
pid_t	fork_and_execute(t_env **env_list, char *command, int fd[2],
			int prev_fd);
void	execute_child_process(t_env **env_list, char *command, int fd[2],
			int prev_fd);
void	execute_parent_process(int fd[2], int *prev_fd);

/*
** This section manages the execution of piped commands. `execute_pipeline`
** splits the command line into commands and initiates their execution.
** `execute_command_loop` iterates over commands, setting up pipes and
** forking processes. `fork_and_execute` creates a process for each
** command in the pipeline. `execute_child_process` sets up redirections
** and runs the command, connecting it appropriately in the pipeline.
** `execute_parent_process` manages file descriptors in the parent process.
** Together, these functions allow the shell to support complex pipelines,
** letting one command's output serve as the input for the next.
*/

/* ************************************************************************** */
/*                          Pipeline Execution Helpers                        */
/* ************************************************************************** */

void	wait_for_children(int num_commands, pid_t *pids, char **commands);
void	setup_pipes(int i, int num_commands, int fd[2]);
void	setup_child_redirections(int prev_fd, int fd[2]);
void	allocate_and_tokenize_args(t_env *env_list, char *command,
			char ***args);
void	execute_command_in_child(t_env **env_list, char **args);

/*
** These helper functions assist in the execution of pipelines.
** `wait_for_children` waits for all child processes to finish and collects
** their exit statuses. `setup_pipes` initializes the pipes needed for
** inter-process communication. `setup_child_redirections` adjusts the
** standard input and output for the child processes based on the pipeline
** configuration. `allocate_and_tokenize_args` prepares the arguments for
** each command by tokenizing them. `execute_command_in_child` decides
** whether to execute a builtin or external command within a child process.
** These functions encapsulate pipeline execution, ensuring each command in
** the pipeline is correctly executed.
*/

/* ************************************************************************** */
/*                          Environment Variables                             */
/* ************************************************************************** */

void	add_env_variable(t_env **env_list, char *name, char *value);
void	load_env(t_env **env_list, char **envp);
char	*get_env_value(t_env *env_list, char *name);
void	set_env_value(t_env **env_list, char *name, char *value);
void	unset_env_variable(t_env **env_list, char *name);

/*
** This section manages the environment variables used by the shell.
** `add_env_variable` adds a new variable to the environment linked list.
** `load_env` initializes the env list from the system-provided `envp` array.
** `get_env_value` retrieves the value of a variable from `env_list`.
** `set_env_value` updates or adds a variable's value in `env_list`.
** `unset_env_variable` removes a variable from the environment.
** By maintaining the environment in a linked list, the shell can easily
** manipulate variables, which is essential for features like variable expansion
** and for passing the correct environment to child processes.
*/

/* ************************************************************************** */
/*                          Environment List Management                       */
/* ************************************************************************** */

void	free_env_list(t_env *env_list);
int		count_env_vars(t_env *env_list);
char	**env_list_to_envp(t_env *env_list);
char	**allocate_envp_array(int count);
void	fill_envp_array(t_env *env_list, char **envp_array);

/*
** These functions handle memory management and conversion of the environment
** linked list. `free_env_list` deallocates all memory used by the environment
** variables. `count_env_vars` returns the number of variables, useful for
** allocating arrays. `env_list_to_envp` converts the linked list into an array
** of strings in the "NAME=VALUE" format, which is necessary when executing
** external programs with `execve`. `allocate_envp_array` allocates memory for
** this array. `fill_envp_array` populates the array with the environment
** variables. These functions ensure that the environment is properly maintained
** and correctly passed to child processes.
*/

/* ************************************************************************** */
/*                          Environment Array Conversion                      */
/* ************************************************************************** */

char	*create_env_string(char *name, char *value);

/*
** This section includes utility functions for handling environment variable
** strings. `create_env_string` constructs a "NAME=VALUE" string from a variable
** name and value, used when converting the environment linked list to an array.
** These utilities support the environment management functions and are crucial
** for passing the environment to executed programs.
*/

/* ************************************************************************** */
/*                          Split & Helpers                                   */
/* ************************************************************************** */

char	**allocate_result(size_t count);
size_t	count_words(const char *s, char c);
int		fill_result_array(const char *s, char c, char **result);
void	free_result(char **result);
char	**ft_split(const char *s, char c);

/*
** These functions are utilities for string manipulation and splitting.
** `ft_split` splits a string into an array based on a delimiter character.
** `allocate_result`, `count_words`, and `fill_result_array` are helper functions
** used by `ft_split` for memory allocation and processing.
** `free_result` frees the memory allocated for the array of strings.
** These functions are essential for tasks like parsing the PATH variable into
** directories and tokenizing input strings.
*/

/* ************************************************************************** */
/*                          Utility Functions_1                               */
/* ************************************************************************** */

int		is_special_char(int c);
int		ft_atoi(const char *str);
int		ft_isalnum(int c);
int		ft_isspace(int c);

/*
** This section provides basic utility functions that replicate common C library
** functions or offer simple utilities. `is_special_char` checks if a character
** is a shell operator. `ft_atoi` converts a string to an integer, handling edge
** cases. `ft_isalnum` and `ft_isspace` check character properties. These
** functions are foundational for input parsing, validation, and variable
** expansion throughout the shell.
*/

/* ************************************************************************** */
/*                          Utility Functions_2                               */
/* ************************************************************************** */

char	*ft_strdup(const char *s1);
char	*ft_strndup(const char *s1, size_t n);
size_t	ft_strlen(const char *s);
size_t	ft_strnlen(const char *s, size_t maxlen);
void	free_split(char **strs);

/*
** Continuing with utility functions, this section includes string manipulation
** functions. `ft_strdup` and `ft_strndup` duplicate strings, with the latter
** limiting the number of characters copied. `ft_strlen` and `ft_strnlen`
** calculate string lengths. `free_split` deallocates memory used by arrays
** created with `ft_split`. These functions are essential for handling dynamic
** strings and arrays throughout the shell, especially in parsing and command
** execution.
*/

/* ************************************************************************** */
/*                          Utility Functions_3                               */
/* ************************************************************************** */

char	*ft_strcpy(char *dest, const char *src);
void	ft_strcat(char *dest, const char *src);
void	*ft_memcpy(void *dest, const void *src, size_t n);
char	*ft_strchr(const char *s, int c);
int		ft_strcmp(const char *s1, const char *s2);

/*
** This section includes additional string and memory functions. `ft_strcpy` and
** `ft_strcat` copy and concatenate strings. `ft_memcpy` copies memory blocks.
** `ft_strchr` locates a character in a string. `ft_strcmp` compares two strings.
** These functions are used in various parts of the shell for manipulating
** strings, such as building paths, comparing command names, and handling
** environment variables.
*/

/* ************************************************************************** */
/*                          Utility Functions_4                               */
/* ************************************************************************** */

int		ft_fileno(FILE *stream);
void	ft_putstr_fd(const char *str, int fd);
void	ft_putnbr_fd(int n, int fd);
void	ft_putchar_fd(char c, int fd);
void	handle_conversion(char c, va_list args, int fd);

/*
** The final utility section provides functions for output operations.
** `ft_fileno` retrieves the file descriptor from a `FILE*` stream.
** `ft_putstr_fd`, `ft_putnbr_fd`, and `ft_putchar_fd` write strings, numbers,
** and characters to a file descriptor, respectively. `handle_conversion` assists
** `ft_fprintf` by handling format specifiers in formatted output. These
** functions are crucial for outputting information to the user and error
** messages, ensuring that all output is properly formatted and directed to the
** correct file descriptors.
*/

/* ************************************************************************** */
/*                          Utility Functions_5                               */
/* ************************************************************************** */

char	*ft_itoa(int nbr);
int		ft_len(long nb);
int		ft_fprintf(FILE *stream, const char *format, ...);

/*
** This section includes additional utility functions. `ft_itoa` converts an
** integer to a string, which is used for expanding variables like `$?`.
** `ft_len` calculates the length needed for integer conversion. `ft_fprintf` is
** a custom implementation of `fprintf` for writing formatted output to a file
** descriptor, useful for error reporting and ensuring consistent output behavior
** across different parts of the shell.
*/

#endif