/**
 * Operating Systems 2014 - Assignment 2
 * Papa Florin, 334CA
 */


#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>

#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#define READ		0
#define WRITE		1
#define SUCCESS		0
#define FAIL		69
#define INITIAL_VALUE 	50
#define FINAL_VALUE		51

#define MAX_SIZE_ENVIRONMENT_VARIABLE 100
#define MAX_THREADS		2

static LPTSTR get_word(word_t *s);

static VOID CloseProcess(LPPROCESS_INFORMATION lppi)
{
	CloseHandle(lppi->hThread);
	CloseHandle(lppi->hProcess);
}

/**
 * Debug method, used by DIE macro.
 */
static VOID PrintLastError(const PCHAR message)
{
	CHAR errBuff[1024];

	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK,
		NULL,
		GetLastError(),
		0,
		errBuff,
		sizeof(errBuff) - 1,
		NULL);

	fprintf(stderr, "%s: %s\n", message, errBuff);
}

/**
 * Internal change-directory command.
 */
static bool shell_cd(word_t *dir, simple_command_t *s)
{
	/* execute cd */
	int status;
	LPTSTR directory;
	
	if(dir == NULL) return SUCCESS;
	
	directory = get_word(dir);
	status = SetCurrentDirectory(directory);
	free(directory);
	
	if(status == 0) {
		return SUCCESS;
	}
	return FAIL;
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
static LPTSTR get_word(word_t *s)
{
	DWORD string_length = 0;
	DWORD substring_length = 0;

	LPTSTR string = NULL;
	CHAR substring[MAX_SIZE_ENVIRONMENT_VARIABLE];

	DWORD dwret;

	while (s != NULL) {
		strcpy(substring, s->string);

		if (s->expand == true) {
			dwret = GetEnvironmentVariable(substring, substring, MAX_SIZE_ENVIRONMENT_VARIABLE);
			if (!dwret)
				/* Environment Variable does not exist. */
				strcpy(substring, "");
		}

		substring_length = strlen(substring);

		string = realloc(string, string_length + substring_length + 1);
		memset(string + string_length, 0, substring_length + 1);

		strcat(string, substring);
		string_length += substring_length;

		s = s->next_part;
	}

	return string;
}

/**
 * Parse arguments in order to successfully process them using CreateProcess
 */
static LPTSTR get_argv(simple_command_t *command)
{
	LPTSTR argv = NULL;
	LPTSTR substring = NULL;
	word_t *param;

	DWORD string_length = 0;
	DWORD substring_length = 0;

	argv = get_word(command->verb);
	assert(argv != NULL);

	string_length = strlen(argv);

	param = command->params;
	while (param != NULL) {
		substring = get_word(param);
		substring_length = strlen(substring);

		argv = realloc(argv, string_length + substring_length + 4);
		assert(argv != NULL);

		strcat(argv, " ");

		/* Surround parameters with ' ' */
		strcat(argv, "'");
		strcat(argv, substring);
		strcat(argv, "'");

		string_length += substring_length + 3;
		param = param->next_word;

		free(substring);
	}

	return argv;
}

/** 
 * redirect handles for main process and print error messages
 * if necessary
 */
VOID print_message(simple_command_t *s, char *command) {
	HANDLE in, out, err;
	HANDLE in_backup, out_backup, err_backup;
	DWORD type;
	int redir_in = -1, redir_out = -1, redir_err = -1;
	char *in_name, *out_name, *err_name;
	int rb;
	SECURITY_ATTRIBUTES sa;

	ZeroMemory(&sa, sizeof(sa));
	sa.bInheritHandle = TRUE;
	
	/* backup for STDIN/STDOUT/STDERR handles */
	in_backup = GetStdHandle(STD_INPUT_HANDLE);
	out_backup = GetStdHandle(STD_OUTPUT_HANDLE);
	err_backup = GetStdHandle(STD_ERROR_HANDLE);
	
	if(s != NULL) {
	
		/* redirect stdin/stdout/stderr */
		if(s->in != NULL) {
			in_name = get_word(s->in);
			in = CreateFile(
				in_name,
				GENERIC_READ,	   /* access mode */
				FILE_SHARE_READ,	   /* sharing option */
				&sa,		   /* security attributes */
				OPEN_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,/* file attributes */
				NULL
			);
			DIE(in == INVALID_HANDLE_VALUE, "Error opening handle\n");
		
			redir_in = 1;
			rb = SetStdHandle(STD_INPUT_HANDLE, in);
			DIE(!rb, "Error redirecting stdin\n");
			free(in_name);
		}
		if(s->out != NULL) {
			out_name = get_word(s->out);
			if((s->io_flags & IO_OUT_APPEND) == IO_REGULAR) {
				type = CREATE_ALWAYS;
			}
			else {
				type = OPEN_ALWAYS;
			}
			
			out = CreateFile(
				out_name,
				GENERIC_WRITE,	   /* access mode */
				FILE_SHARE_READ,	   /* sharing option */
				&sa,		   /* security attributes */
				type,
				FILE_ATTRIBUTE_NORMAL,/* file attributes */
				NULL
			);
			DIE(out == INVALID_HANDLE_VALUE, "Error opening handle\n");
			
			/* append mode */
			if(type == OPEN_ALWAYS) {
				SetFilePointer(out, 0, NULL, FILE_END);
			}
			
			redir_out = 1;
			rb = SetStdHandle(STD_OUTPUT_HANDLE, out);
			DIE(!rb, "Error redirecting stout\n");
			free(out_name);
		}
		if(s->err != NULL) {
			err_name = get_word(s->err);
			out_name = get_word(s->out);	
			
			if(out_name != NULL && strcmp(err_name, out_name) == 0) {
				err = out;
				redir_err = 2;
			}
			else {
				if((s->io_flags & IO_ERR_APPEND) == IO_REGULAR) {
					type = CREATE_ALWAYS;
				}
				else {
					type = OPEN_ALWAYS;
				}
				
				err = CreateFile(
					err_name,
					GENERIC_WRITE,	   /* access mode */
					FILE_SHARE_READ,	   /* sharing option */
					&sa,		   /* security attributes */
					type,
					FILE_ATTRIBUTE_NORMAL,/* file attributes */
					NULL
				);
				DIE(err == INVALID_HANDLE_VALUE, "Error opening handle\n");
				
				/* append mode */
				if(type == OPEN_ALWAYS) {
					SetFilePointer(err, 0, NULL, FILE_END);
				}
				redir_err = 1;
			}
			
			rb = SetStdHandle(STD_ERROR_HANDLE, err);
			DIE(!rb, "Error redirecting stderr\n");
			
			free(err_name);
			if(out_name != NULL) {
				free(out_name);
			}
		}
		
		if(command != NULL) {
			fprintf(stderr, "Execution failed for '%s'\n", command);
			fflush(stderr);
		}
		
		/* restore stdin/stdout/stderr */
		if(redir_in == 1) {
			rb = SetStdHandle(STD_INPUT_HANDLE, in_backup);
			DIE(!rb, "Error redirecting stdin\n");
			
			rb = CloseHandle(in);
			DIE(!rb, "Error closing stdin\n");
		}
		
		if(redir_out == 1) {
			rb = SetStdHandle(STD_OUTPUT_HANDLE, out_backup);
			DIE(!rb, "Error redirecting stdout\n");
			
			rb = CloseHandle(out);
			DIE(!rb, "Error closing stdout\n");
		}
		
		if(redir_err != -1) {
			rb = SetStdHandle(STD_ERROR_HANDLE, err_backup);
			DIE(!rb, "Error redirecting stderr\n");
			
			if(redir_err == 1) {
				rb = CloseHandle(err);
				DIE(!rb, "Error closing stderr\n");
			}
		}
	}
}

/* redirect handle from STDIN/STDOUT/STDERR */
static VOID RedirectHandle(STARTUPINFO *psi, HANDLE hFile, INT opt)
{

	if (hFile == INVALID_HANDLE_VALUE)
		return;

	/* Redirect one of STDIN, STDOUT, STDERR to hFile */
	
	switch (opt) {
		case STD_INPUT_HANDLE:
			psi->hStdInput = hFile;
			break;
		case STD_OUTPUT_HANDLE:
			psi->hStdOutput = hFile;
			break;
		case STD_ERROR_HANDLE:
			
			psi->hStdError = hFile;
			break;
	}
}

/* redirect everything */
void redirect_all(simple_command_t *s, STARTUPINFO *si, HANDLE *in, HANDLE *out, HANDLE *err) {
	DWORD type;
	int redir_in = -1, redir_out = -1, redir_err = -1;
	char *in_name, *out_name, *err_name;
	SECURITY_ATTRIBUTES sa;

	ZeroMemory(&sa, sizeof(sa));
	sa.bInheritHandle = TRUE;
	
	ZeroMemory(si, sizeof(*si));
	si->cb = sizeof(*si);
	
	/* set handles from psi to current STDIN, STDOUT, STDERR handles */
	si->hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	si->hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	si->hStdError = GetStdHandle(STD_ERROR_HANDLE);

	si->dwFlags |= STARTF_USESTDHANDLES;
	
	if(s != NULL) {
	
		/* redirect stdin/stdout/stderr */
		if(s->in != NULL) {
			in_name = get_word(s->in);
			*in = CreateFile(
				in_name,
				GENERIC_READ,	   /* access mode */
				FILE_SHARE_READ,	   /* sharing option */
				&sa,		   /* security attributes */
				OPEN_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,/* file attributes */
				NULL
			);
			DIE(*in == INVALID_HANDLE_VALUE, "Error opening handle\n");
		
			redir_in = 1;
			RedirectHandle(si, *in, STD_INPUT_HANDLE);
			free(in_name);
		}
		if(s->out != NULL) {
			out_name = get_word(s->out);
			if((s->io_flags & IO_OUT_APPEND) == IO_REGULAR) {
				type = CREATE_ALWAYS;
			}
			else {
				type = OPEN_ALWAYS;
			}
			
			*out = CreateFile(
				out_name,
				GENERIC_WRITE,	   /* access mode */
				FILE_SHARE_READ,	   /* sharing option */
				&sa,		   /* security attributes */
				type,
				FILE_ATTRIBUTE_NORMAL,/* file attributes */
				NULL
			);
			DIE(*out == INVALID_HANDLE_VALUE, "Error opening handle\n");
			
			/* append mode */
			if(type == OPEN_ALWAYS) {
				SetFilePointer(*out, 0, NULL, FILE_END);
			}
			
			redir_out = 1;
			RedirectHandle(si, *out, STD_OUTPUT_HANDLE);
			free(out_name);
		}
		if(s->err != NULL) {
			err_name = get_word(s->err);
			out_name = get_word(s->out);
			
			if(out_name != NULL && strcmp(err_name, out_name) == 0) {
				*err = *out;
				redir_err = 2;
			}
			else {
				if((s->io_flags & IO_ERR_APPEND) == IO_REGULAR) {
					//fflush(stderr);
					type = CREATE_ALWAYS;
				}
				else {
					//fflush(stderr);
					type = OPEN_ALWAYS;
				}
				
				*err = CreateFile(
					err_name,
					GENERIC_WRITE,	   /* access mode */
					FILE_SHARE_READ,	   /* sharing option */
					&sa,		   /* security attributes */
					type,
					FILE_ATTRIBUTE_NORMAL,/* file attributes */
					NULL
				);
				DIE(*err == INVALID_HANDLE_VALUE, "Error opening handle\n");
				
				/* append mode */
				if(type == OPEN_ALWAYS) {
					SetFilePointer(*err, 0, NULL, FILE_END);
				}
				redir_err = 1;
			}
			
			RedirectHandle(si, *err, STD_ERROR_HANDLE);
			
			free(err_name);
			if(out_name != NULL) {
				free(out_name);
			}
		}
	}
}

/**
 * Parse and execute a simple command, by either creating a new processing or
 * internally process it.
 */
bool parse_simple(simple_command_t *s, int level, command_t *father, void *h)
{
	LPTSTR command;
	PCHAR argv;
	HANDLE in = INVALID_HANDLE_VALUE, out = INVALID_HANDLE_VALUE, err = INVALID_HANDLE_VALUE;
	DWORD dwRet, bRet, bRes, dwRes;
	char *var, *value;
	int status, internal = 0;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	
	if(s == NULL || s->verb == NULL) return SUCCESS;

	command = get_word(s->verb);
	
	/* if built-in command, execute the command */
	if(strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0) {
		return shell_exit();
	}
	else if(strcmp(command, "cd") == 0) {
		status = shell_cd(s->params, s);
		internal = 1;
	}
	else if(strchr(command, '=') != NULL) {
		internal = 1;
		var = strtok(command, "=");
		value = strtok(NULL, "=");
		if(var != NULL && value != NULL) {
			if(SetEnvironmentVariable(var, value)) {
				status = SUCCESS;
			}
			else {
				status = FAIL;
			}
		}
		else {
			status = SUCCESS;
		}
	}
	/* external */
	else {
		/* redirect input/output/error */
		argv = get_argv(s);
		
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		
		redirect_all(s, &si, &in, &out, &err);
		
		bRet = CreateProcess(
			 NULL,          /* No module name */
			 argv,       /* Command line */
			 NULL,          /* Process handle not inheritable */
			 NULL,          /* Thread handle not inheritable */
			 TRUE,         /* Set handle inheritance */
			 0,             /* No creation flags */
			 NULL,          /* Use parent's environment block */
			 NULL,          /* Use parent's starting directory */
			 &si,           /* Pointer to STARTUPINFO structure */
			 &pi);          /* Pointer to PROCESS_INFORMATION */
		if(bRet != FALSE) {
			dwRet = WaitForSingleObject(pi.hProcess, INFINITE);
			DIE(dwRet == WAIT_FAILED, "WaitForSingleObject");
		
			bRes = GetExitCodeProcess(pi.hProcess, &dwRes);
			DIE(bRes == FALSE, "GetExitCode");

			CloseProcess(&pi);
			status = dwRes;
		}
		else {
			status = FAIL;
		}
		
		if(in != INVALID_HANDLE_VALUE) {
			bRet = CloseHandle(in); 
			DIE(bRet == FALSE, "CloseHandle");
		}
		if(out != INVALID_HANDLE_VALUE) {
			bRet = CloseHandle(out); 
			DIE(bRet == FALSE, "CloseHandle");
		}
		if(err != INVALID_HANDLE_VALUE && err != out) {
			bRet = CloseHandle(err); 
			DIE(bRet == FALSE, "CloseHandle");
		}
		
		free(argv);
	}
		
	if(status == FAIL) {
		/* prints error message and performs redirects */
		print_message(s, command);
	}
	else if(internal){
		print_message(s, NULL);
	}

 	free(command);

	return status;
}

DWORD WINAPI my_parse_command(command_t *cmd);

/**
 * Process two commands in parallel, by creating two children.
 */
static bool do_in_parallel(command_t *cmd1, command_t *cmd2, int level, command_t *father)
{
	int status1, status2, i, rb;
	command_t *cmd;
    DWORD   dwThreadIdArray[MAX_THREADS];
    HANDLE  hThreadArray[MAX_THREADS];
	
	for(i = 0; i < MAX_THREADS; i++)
    {
        // Create the thread to begin execution on its own.
		
		if(i == 0) {
			cmd = cmd1;
		}
		else {
			cmd = cmd2;
		}
		
        hThreadArray[i] = CreateThread( 
            NULL,                   // default security attributes
            0,                      // use default stack size  
            my_parse_command,       // thread function name
            cmd,          // argument to thread function 
            0,                      // use default creation flags 
            &dwThreadIdArray[i]);   // returns the thread identifier 


        // Check the return value for success.
        // If CreateThread fails, terminate execution. 
        // This will automatically clean up threads and memory. 
		DIE(hThreadArray[i] == NULL, "Error creating thread\n");
		
    } // End of main thread creation loop.

    // Wait until all threads have terminated.

    WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);

    // Close all thread handles and free memory allocations.

	rb = GetExitCodeThread(hThreadArray[0], &status1);
	DIE(!rb, "Error getting thread status\n");
	
	rb = GetExitCodeThread(hThreadArray[1], &status2);
	DIE(!rb, "Error getting thread status\n");
	
    for(i = 0; i < MAX_THREADS; i++)
    {
        CloseHandle(hThreadArray[i]);
    }

	if(status1 != 0 && status2 != 0) {
		return 1;
	}
	else {
		return 0;
	}
}

/**
 * Run commands by creating an anonymous pipe (cmd1 | cmd2)
 */
static bool do_on_pipe(command_t *cmd1, command_t *cmd2, int level, command_t *father)
{
	HANDLE hRead, hWrite, out_backup, in_backup;
	SECURITY_ATTRIBUTES sa;
	DWORD rb;
	int status1, status2;
	
	in_backup = GetStdHandle(STD_INPUT_HANDLE);
	out_backup = GetStdHandle(STD_OUTPUT_HANDLE);
	
	/* create pipe */
	ZeroMemory(&sa, sizeof(sa));
	sa.bInheritHandle=TRUE;
	CreatePipe(
		&hRead, 
		&hWrite,
		&sa,        //pentru moștenire sa.bInheritHandle=TRUE
		11000000           //dimensiunea default pentru pipe
	);
	
	/* scriere in pipe */
	rb = SetStdHandle(STD_OUTPUT_HANDLE, hWrite);
	DIE(!rb, "Error redirecting stdout\n");
	
	status1 = parse_command(cmd1, level, father, father);
	
	/* restore stdout */
	rb = SetStdHandle(STD_OUTPUT_HANDLE, out_backup);
	DIE(!rb, "Error redirecting stdout\n");
	rb = CloseHandle(hWrite);
	DIE(!rb, "Error closing pipe\n");
	
	/* citire din pipe */
	rb = SetStdHandle(STD_INPUT_HANDLE, hRead);
	DIE(!rb, "Error redirecting stdin\n");
	
	status2 = parse_command(cmd2, level, father, father);
	
	/* restore stdin */
	rb = SetStdHandle(STD_INPUT_HANDLE, in_backup);
	DIE(!rb, "Error redirecting stdin\n");
	rb = CloseHandle(hRead);
	DIE(!rb, "Error closing pipe\n");

	return status2;
}

/**
 * Parse and execute a command.
 */
int parse_command(command_t *c, int level, command_t *father, void *h)
{
	if(c == NULL) {
		return 0;
	}
	
	if (c->op == OP_NONE) {
		return parse_simple(c->scmd, level, father, h);
	}

	switch (c->op) {
	case OP_SEQUENTIAL:
		parse_command(c->cmd1, level + 1, c, h);
		return parse_command(c->cmd2, level + 1, c, h);
		break;

	case OP_PARALLEL:
		return do_in_parallel(c->cmd1, c->cmd2, level + 1, h);
		break;

	case OP_CONDITIONAL_NZERO:
		if(parse_command(c->cmd1, level + 1, c, h) != SUCCESS) {
			return parse_command(c->cmd2, level + 1, c, h);
		}
		break;

	case OP_CONDITIONAL_ZERO:
		if(parse_command(c->cmd1, level + 1, c, h) == SUCCESS) {
			return parse_command(c->cmd2, level + 1, c, h);
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

/* used for thread creation */
DWORD WINAPI my_parse_command(command_t *cmd) {
	return parse_command(cmd, 0, NULL, NULL);
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

	chunk = calloc(CHUNK_SIZE, sizeof(char));
	if (chunk == NULL) {
		fprintf(stderr, ERR_ALLOCATION);
		exit(EXIT_FAILURE);
	}

	instr = NULL;
	instr_length = 0;

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

		instr = realloc(instr, instr_length + CHUNK_SIZE);
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
