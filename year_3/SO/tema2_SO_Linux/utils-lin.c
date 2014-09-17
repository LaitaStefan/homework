/**
 * Papa Florin, 334CA
 * Operating Sytems 2014 - Assignment 2
 */

#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <unistd.h>

#include "utils.h"

#define SUCCESS		0
#define READ		0
#define WRITE		1
#define REPLACE 	1
#define PIPE_NO 	2
#define ERROR 		-1

/* exit with message */
void DIE(int condition, char *message) {
	if(condition) {
		fprintf(stderr, "%s\n", message);
		exit(EXIT_FAILURE);
	}
}

static char *get_word(word_t *s);

/**
 * Internal change-directory command.
 */
static bool shell_cd(word_t *dir)
{
	char *directory;
	int res;

	if(dir != NULL) {
		directory = get_word(dir);
		if(directory != NULL) {
			res = chdir(directory); 
		}
		else {
			res = chdir(directory);
		}

		free(directory);

		if(res == 0) {
			return true;
		}
	}

	return false;
}

/**
 * Internal exit/quit command.
 */
static int shell_exit()
{
	return SHELL_EXIT;
}

/**
 * Concatenate parts of the word to obtain the command
 */
static char *get_word(word_t *s)
{
	int string_length = 0;
	int substring_length = 0;

	char *string = NULL;
	char *substring = NULL;

	while (s != NULL) {
		substring = strdup(s->string);

		if (substring == NULL) {
			return NULL;
		}

		if (s->expand == true) {
			char *aux = substring;
			substring = getenv(substring);

			/* prevents strlen from failing */
			if (substring == NULL) {
				substring = calloc(1, sizeof(char));
				if (substring == NULL) {
					free(aux);
					return NULL;
				}
			}

			free(aux);
		}

		substring_length = strlen(substring);

		string = realloc(string, string_length + substring_length + 1);
		if (string == NULL) {
			if (substring != NULL)
				free(substring);
			return NULL;
		}

		memset(string + string_length, 0, substring_length + 1);

		strcat(string, substring);
		string_length += substring_length;

		if (s->expand == false) {
			free(substring);
		}

		s = s->next_part;
	}

	return string;
}

/**
 * Concatenate command arguments in a NULL terminated list in order to pass
 * them directly to execv.
 */
static char **get_argv(simple_command_t *command, int *size)
{
	char **argv;
	word_t *param;

	int argc = 0;
	argv = calloc(argc + 1, sizeof(char *));
	assert(argv != NULL);

	argv[argc] = get_word(command->verb);
	assert(argv[argc] != NULL);

	argc++;

	param = command->params;
	while (param != NULL) {
		argv = realloc(argv, (argc + 1) * sizeof(char *));
		assert(argv != NULL);

		argv[argc] = get_word(param);
		assert(argv[argc] != NULL);

		param = param->next_word;
		argc++;
	}

	argv = realloc(argv, (argc + 1) * sizeof(char *));
	assert(argv != NULL);

	argv[argc] = NULL;
	*size = argc;

	return argv;
}

/* perform redirects */
void perform_redirects(simple_command_t *s, int *fd_s, int *fd_d, int *fd_e) {
	int rc;
	char *dest, *source, *err;

	if(s != NULL) {
		if(s->out != NULL) {
			dest = get_word(s->out);
			if(s->io_flags == IO_REGULAR) {
				*fd_d = open(dest, O_WRONLY|O_CREAT|O_TRUNC, 0644);
			}
			else {
				*fd_d = open(dest, O_WRONLY|O_CREAT|O_APPEND, 0644);
			}
            DIE(*fd_d < 0, "nu s-a putut deschide fisierul");

            free(dest);
 
            rc = close(STDOUT_FILENO);
            DIE(rc < 0, "close stdout");
 
            rc = dup2(*fd_d, STDOUT_FILENO);
            DIE(rc < 0, "dup2 fd_dst");
		}

		if(s->in != NULL) {
			source = get_word(s->in);
			*fd_s = open(source, O_RDONLY, 0644);
            DIE(*fd_s < 0, "nu s-a putut deschide fisierul");

            free(source);

            rc = close(STDIN_FILENO);
            DIE(rc < 0, "close stdin");
 
            rc = dup2(*fd_s, STDIN_FILENO);
            DIE(rc < 0, "dup2 fd_src");
		}

		if(s->err != NULL) {
			err = get_word(s->err);
			dest = get_word(s->out);

			if(s->out != NULL && strcmp(err, dest) == 0) {
				*fd_e = *fd_d;
			}
			else {
				if(s->io_flags == IO_REGULAR) {
					*fd_e = open(err, O_WRONLY|O_CREAT|O_TRUNC, 0644);
				}
				else {
					*fd_e = open(err, O_WRONLY|O_CREAT|O_APPEND, 0644);
				}
			}
            DIE(*fd_e < 0, "nu s-a putut deschide fisierul");

            free(err);
            if(dest != NULL) {
            	free(dest);
            }
 
            rc = close(STDERR_FILENO);
            DIE(rc < 0, "close stderr");
 
            rc = dup2(*fd_e, STDERR_FILENO);
            DIE(rc < 0, "dup2 fd_err");
		}
	}
}

/**
 * print error message and perform redirects
 * in case printing an error message is required
 **/
void print_message(simple_command_t *s, char *command) {
	int backup_in = -1, backup_out = -1, backup_err = -1;
	int fd_src = -1, fd_dst = -1, fd_err = -1, rc;

	if(s != NULL) {
		/* save STDIN for backup */
		backup_in = dup(STDIN_FILENO);
		DIE(backup_in < 0, "nu s-a putut face dup\n");

		/* save STDOUT for backup */
		backup_out = dup(STDOUT_FILENO);
		DIE(backup_out < 0, "nu s-a putut face dup\n");

		/* save STDERR for backup */
		backup_err = dup(STDERR_FILENO);
		DIE(backup_err < 0, "nu s-a putut face dup\n");

		perform_redirects(s, &fd_src, &fd_dst, &fd_err);
	}

	fprintf(stderr, "Execution failed for '%s'\n", command);

	/* restore STDIN/STDOUT/STDERR and close used file descriptors*/
	if(backup_in != -1) {
		dup2(backup_in, STDIN_FILENO);
		DIE(backup_in < 0, "nu s-a putut dace dup\n");

		rc = close(backup_in);
		DIE(rc < 0, "eror closing backup_in\n");
	}

	if(backup_out != -1) {
		dup2(backup_out, STDOUT_FILENO);
		DIE(backup_out < 0, "nu s-a putut dace dup\n");

		rc = close(backup_out);
		DIE(rc < 0, "eror closing backup_out\n");
	}

	if(backup_err != -1) {
		dup2(backup_err, STDERR_FILENO);
		DIE(backup_err < 0, "nu s-a putut dace dup\n");

		rc = close(backup_err);
		DIE(rc < 0, "eror closing backup_err\n");
	}

	if(fd_src != -1) {
		rc = close(fd_src);
		DIE(fd_src < 0, "eror closing src\n");
	}

	if(fd_dst != -1) {
		rc = close(fd_dst);
		DIE(rc < 0, "eror closing dst\n");
	}

	if(fd_err != -1) {
		rc = close(fd_err);
		DIE(rc < 0, "eror closing err\n");
	}
}

/* free argv memory */
void free_mem(char **argv, int size) {
	int i;

	for(i = 0; i < size; i++) {
		free(argv[i]);
	}

	free(argv);
}

/**
 * Parse a simple command (internal, environment variable assignment,
 * external command).
 */
static int parse_simple(simple_command_t *s, int level, command_t *father)
{
	char *command, *name, *value;
	pid_t pid;
	int status, size;
	int fd_dst = -1, fd_src = -1, fd_err = -1, rc;
	int internal = 0;
	char **argv;

	if(s == NULL || s->verb == NULL) {
		return 0;
	}

	/* if built-in command, execute the command */
	command = get_word(s->verb);

	if(strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0) {
		status = shell_exit();
	}
	else if(strcmp(command, "cd") == 0) {
		internal = 1;
		if(shell_cd(s->params)) {
			status = SUCCESS;
		}
		else {	//caz de eroare
			status = EXIT_FAILURE;
		}
	}
	/* asignare de variabila */
	else if(strchr(command, '=') != NULL) {
		internal = 1;
		name = strtok(command, "=");
		value = strtok(NULL, "=");
		if(name != NULL && value != NULL) {
			status = setenv(name, value, REPLACE);
		}
	}
	/* execute external command */
	else {
	    argv = get_argv(s, &size);
		pid = fork();
		switch (pid) {
			case -1:
				/* error forking */
				return EXIT_FAILURE;
		 
			case 0:
				/* redirect if necessary */
				perform_redirects(s, &fd_src, &fd_dst, &fd_err);

				/* child process */
				if(execvp(command, (char *const *) argv) == ERROR) {
					print_message(NULL, command);
				}

				if(fd_src != -1) {
					rc = close(fd_src);
					DIE(rc < 0, "error closing source file");
				}

				if(fd_dst != -1) {
					rc = close(fd_dst);
					DIE(rc < 0, "error closing source file");
				}

				if(fd_err != -1) {
					rc = close(fd_err);
					DIE(rc < 0, "error closing source file");
				}
		 
				/* only if exec failed */
				exit(EXIT_FAILURE);
		 
			default:
				/* parent process */
				break;
		}
 
		/* only parent process gets here */
		waitpid(pid, &status, 0);
		free_mem(argv, size);
	}
 	
 	if(internal && status != SUCCESS) {
 		/* prints error message and performs redirects */
 		print_message(s, command);
 	}

 	free(command);

	return status;
}

/* fork process and parse command */
int launch(command_t *cmd, int level, command_t *father, pid_t *pid) {
	*pid = fork();
	switch (*pid) {
		case -1:
			/* error forking */
			return EXIT_FAILURE;

		case 0:
			/* child process */
			parse_command(cmd, level, father);
	 
			/* only if exec failed */
			exit(EXIT_FAILURE);
	 
		default:
			/* parent process */
			break;
	}

	return 0;
}

/**
 * Process two commands in parallel, by creating two children.
 */
static bool do_in_parallel(command_t *cmd1,
						   command_t *cmd2,
						   int level, 
						   command_t *father)
{
	int status1, status2;
	pid_t pid1, pid2;

	/* launch both threads and wait later */
	status1 = launch(cmd1, level, father, &pid1);
	status2 = launch(cmd2, level, father, &pid2);

	/* wait for threads */
	if(status1 != EXIT_FAILURE) {
		waitpid(pid1, &status1, 0);
	}
	if(status2 != EXIT_FAILURE) {
		waitpid(pid2, &status2, 0);
	}

	return status1 & status2;
}

/**
 * Run commands by creating an anonymous pipe (cmd1 | cmd2)
 */
static bool do_on_pipe(command_t *cmd1,
					   command_t *cmd2,
					   int level,
					   command_t *father)
{
	int filedes[PIPE_NO];
	pid_t pid1, pid2;
	int status1, status2, rc;

	if(pipe(filedes)) {
		fprintf(stderr, "Pipe creation failed\n");
		return EXIT_FAILURE;
	}

	pid1 = fork();
	switch (pid1) {
		case -1:
			/* error forking */
			return EXIT_FAILURE;

		case 0:
			/* redirect stdout */
			rc = close(STDOUT_FILENO);
            DIE(rc < 0, "close stdout");
 
            rc = dup2(filedes[1], STDOUT_FILENO);
            DIE(rc < 0, "dup2 filedes[1]");

			/* child process */
			parse_command(cmd1, level, father);
	 
			/* only if exec failed */
			exit(EXIT_FAILURE);
	 
		default:
			/* parent process */
			break;
	}

	rc = close(filedes[1]);
	DIE(rc < 0, "close filedes[1]");	

	pid2 = fork();
	switch (pid2) {
		case -1:
			/* error forking */
			return EXIT_FAILURE;

		case 0:
			/* redirect stdin */
			rc = close(STDIN_FILENO);
            DIE(rc < 0, "close stdin");
 
            rc = dup2(filedes[0], STDIN_FILENO);
            DIE(rc < 0, "dup2 filedes[0]");

			/* child process */
			parse_command(cmd2, level, father);
	 
			/* only if exec failed */
			exit(EXIT_FAILURE);
	 
		default:
			/* parent process */
			break;
	}

	rc = close(filedes[0]);
	DIE(rc < 0, "close filedes[0]");

	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	
	if(WIFEXITED(status2)) {
		return SUCCESS;
	}

	return status2;
}

/**
 * Parse and execute a command.
 */
int parse_command(command_t *c, int level, command_t *father)
{
	if(c == NULL) {
		return 0;
	}

	if (c->op == OP_NONE) {
		return parse_simple(c->scmd, level, father);
	}

	switch (c->op) {
	case OP_SEQUENTIAL:
		parse_command(c->cmd1, level + 1, c);
		return parse_command(c->cmd2, level + 1, c);
		break;

	case OP_PARALLEL:
		return do_in_parallel(c->cmd1, c->cmd2, level + 1, c);
		break;

	case OP_CONDITIONAL_NZERO:
		if(parse_command(c->cmd1, level + 1, c)) {
			return parse_command(c->cmd2, level + 1, c);
		}
		break;

	case OP_CONDITIONAL_ZERO:
		if(!parse_command(c->cmd1, level + 1, c)) {
			return parse_command(c->cmd2, level + 1, c);
		}
		break;

	case OP_PIPE:
		return do_on_pipe(c->cmd1, c->cmd2, level + 1, c);
		break;

	default:
		assert(false);
	}

	return SUCCESS;
}

/**
 * Readline from mini-shell.
 */
char *read_line()
{
	char *instr;
	char *chunk;
	char *ret;

	int instr_length;
	int chunk_length;

	int endline = 0;

	instr = NULL;
	instr_length = 0;

	chunk = calloc(CHUNK_SIZE, sizeof(char));
	if (chunk == NULL) {
		fprintf(stderr, ERR_ALLOCATION);
		return instr;
	}

	while (!endline) {
		ret = fgets(chunk, CHUNK_SIZE, stdin);
		if (ret == NULL) {
			break;
		}

		chunk_length = strlen(chunk);
		if (chunk[chunk_length - 1] == '\n') {
			chunk[chunk_length - 1] = 0;
			endline = 1;
		}

		ret = instr;
		instr = realloc(instr, instr_length + CHUNK_SIZE + 1);
		if (instr == NULL) {
			free(ret);
			return instr;
		}
		memset(instr + instr_length, 0, CHUNK_SIZE);
		strcat(instr, chunk);
		instr_length += chunk_length;
	}

	free(chunk);

	return instr;
}
