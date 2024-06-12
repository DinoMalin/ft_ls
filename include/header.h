#pragma once

#include "libft.h"
#include <stdlib.h>
#include "errors.h"
#include <stdbool.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

typedef enum {
	OPTION		= 1 << 0,
	LONG_OPTION	= 1 << 1,
	ARG			= 1 << 2,
} Arg_type;

typedef enum {
	list			= 1 << 0,
	recursive		= 1 << 1,
	all				= 1 << 2,
	reverse			= 1 << 3,
	time			= 1 << 4,
	help			= 1 << 5,
	basic_display	= 1 << 6,
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
	Flag	flags;

	int		size;
	int		nb_file;
	char	*perm_errors;
} Command;


/* === PARSING === */
void	get_flags(Command *cmd);
bool	fatal_error(Command *cmd);
Command	get_cmd(int ac, char **av);

/* === EXECUTION === */
void	pre_treatment(Arg *arg, Command *cmd);
int		ft_ls(const char *path, Command *cmd);
