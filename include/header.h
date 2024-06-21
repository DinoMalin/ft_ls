#pragma once

#include "libft.h"
#include "errors.h"
#include "help.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <linux/limits.h>
#include <locale.h>

#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>

#define DIR_COLOR	"\e[1;34m"
#define LN_COLOR	"\e[1;36m"
#define EX_COLOR	"\e[1;32m"
#define OR_COLOR	"\e[1;31m"
#define RESET		"\e[0m"

#define COLOR(type)	  type == DIRECTORY		? DIR_COLOR \
					: type == SYMLINK		? LN_COLOR \
					: type == EXECUTABLE	? EX_COLOR \
					: type == DEAD_LINK		? OR_COLOR \
					: ""

typedef enum {
	NOERROR,
	NOSUCHFILE,
	NOPERM
} Error;

typedef enum {
	OPTION		= 1 << 0,
	LONG_OPTION	= 1 << 1,
	ARG			= 1 << 2,
} Arg_type;

typedef enum {
	DIRECTORY,
	REGULAR_FILE,
	SYMLINK,
	DEAD_LINK,
	EXECUTABLE
} FileType;

typedef enum {
	long_display	= 1 << 0,
	recursive		= 1 << 1,
	all				= 1 << 2,
	reverse			= 1 << 3,
	time_modif		= 1 << 4,
	help			= 1 << 5,
	basic_display	= 1 << 6,
	commas			= 1 << 7,
	quotes			= 1 << 8,
	no_owner		= 1 << 9,
	dir_only		= 1 << 10,
	dotfiles		= 1 << 11,
} Flag;

typedef struct {
	Arg_type	type;
	char		*content;
	bool		error;
} Arg;

typedef struct {
	size_t	link;
	size_t	owner;
	size_t	group;
	size_t	size;
} Size;

typedef struct File {
	FileType		type;
	struct File		**childs;
	Error			error;
	int				nb_childs;
	Size			size_childs;

	char			link_to[PATH_MAX];
	FileType		link_type;

	char			*path;
	char			*name;
	time_t			last_modif;
	char			permissions[11];
	char			last_modif_str[13];
	char			*nb_links;
	char			*owner;
	char			*group;
	char			*size;
	int				blocks;
	int				total;
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
void	free_file(File *file, bool long_display);
void	free_childs(File *node, bool long_display);
int		free_command(Command *cmd);
void	display(Command *cmd, File *node);
void	display_file(Command *cmd, File *node, Size *size, bool last);
void	calculate_size(Size *size, File *node);
char	*clean_join(char *origin, const char *to_join);
void	analyze_file(File *file, bool long_display);

/* === PARSING === */
void	get_flags(Command *cmd);
bool	fatal_error(Command *cmd);
Command	*init_cmd(int ac, char **av);

/* === EXECUTION === */
void	add_file_to_link(File *link);
void	add_to_file_system(File *parent, struct dirent *entry, bool long_display);
void	ft_ls(Command *cmd, File *parent);

/* === SORT === */
int		compare_name(File *a, File *b);
int		compare_time(File *a, File *b);
void	sort(File **arr, int size, int (*compare)(File *a, File *b));
