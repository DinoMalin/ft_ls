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
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include "help.h"

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
	long_display	= 1 << 0,
	recursive		= 1 << 1,
	all				= 1 << 2,
	reverse			= 1 << 3,
	time_modif		= 1 << 4,
	help			= 1 << 5,
	basic_display	= 1 << 6,
	commas			= 1 << 7
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
	char			*path;

	char			*name;
	char			*error;
	int				nb_childs;
	time_t			last_modif;
	char			permissions[11];
	char			last_modif_str[13];
	int				nb_links;
	char			*owner;
	char			*group;
	int				size;
	int				blocks;
} File;

typedef struct {
	Arg		*args;
	Flag	flags;
	File	**file_system;

	int		size;
	int		nb_file;
	bool	displayed;

	int		return_status;
} Command;


/* === UTILS === */
void	free_file(File *file);
void	display(Command *cmd, File *node);
char	*clean_join(char *origin, const char *to_join);
void	analyze_file(File *file);

/* === PARSING === */
void	get_flags(Command *cmd);
bool	fatal_error(Command *cmd);
Command	*init_cmd(int ac, char **av);

/* === EXECUTION === */
void	add_to_file_system(File *parent, struct dirent *entry);
int		ft_ls(Command *cmd, File *parent);

/* === SORT === */
int		compare_name(File *a, File *b);
int		compare_time(File *a, File *b);
void	sort(File **arr, int size, int (*compare)(File *a, File *b));
