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
#include <sys/stat.h>

#define DIR_COLOR	"\e[1;34m"
#define RESET		"\e[0m"

typedef enum {
	OPTION		= 1 << 0,
	LONG_OPTION	= 1 << 1,
	ARG			= 1 << 2,
} Arg_type;

typedef enum {
	DIRECTORY,
	REGULAR_FILE,
	SYMLINK
} FileType;

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

typedef struct File {
	FileType		type;
	struct File		**childs;

	char			*name;
	char			*path;
	char			*error;

	int				nb_childs;
	int				level;
} File;

typedef struct {
	Arg		*args;
	Flag	flags;
	File	**file_system;

	int		size;
	int		nb_file;
	int		last_file;
	char	*perm_errors;
	bool	displayed;
} Command;


/* === UTILS === */
char *clean_join(char *origin, const char *to_join);

/* === PARSING === */
void	get_flags(Command *cmd);
bool	fatal_error(Command *cmd);
Command	*init_cmd(int ac, char **av);

/* === EXECUTION === */
void	pre_treatment(File *file, Command *cmd);
void	add_to_file_system(File *parent, struct dirent *entry);
int		ft_ls(Command *cmd, File *parent);

/* === SORT === */
int	compare_name(File *a, File *b);
void sort(File *parent, int (*compare)(File *a, File *b));
