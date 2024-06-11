#pragma once

#include "libft.h"
#include <stdlib.h>
#include "errors.h"
#include <stdbool.h>

typedef enum {
	OPTION		= 1 << 0,
	LONG_OPTION	= 1 << 1,
	ARG			= 1 << 2,
	FILE		= 1 << 3,
	FOLDER		= 1 << 4
} Arg_type;

typedef enum {
	list		= 1 << 0,
	recursive	= 1 << 1,
	all			= 1 << 2,
	reverse		= 1 << 3,
	time		= 1 << 4,
	help		= 1 << 5,
} Flag;

typedef struct {
	int importance;
	int index;
} Error;

typedef struct {
	Arg_type	type;
	char		*content;
	Error		error;
} Arg;

typedef struct {
	Arg		*args;
	int		size;
	Flag	flags;
} Command;


/* === PARSING === */
void	get_flags(Command *cmd);
bool	fatal_error(Command *cmd);
Command	get_cmd(int ac, char **av);
