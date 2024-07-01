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
#include <sys/ioctl.h>

#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>

#define RESET		"\e[0m"

typedef enum {
	NOERROR,
	NOSUCHFILE,
	NOPERM,
	STAT,
} Error;

typedef enum {
	OPTION		= 1 << 0,
	LONG_OPTION	= 1 << 1,
	ARG			= 1 << 2,
} ArgType;

typedef enum {
	REGULAR_FILE,
	DIRECTORY,
	SYMLINK,
	EXECUTABLE,
	CHARACTER,
	BLOCK,
	PIPE,
	SOCKET,
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
	colors			= 1 << 12,
} Flag;

typedef struct {
	ArgType	type;
	char		*content;
	bool		error;
} Arg;

typedef struct {
	int	*files;
	int	size;
} Col;

typedef struct {
	int		max_el;
	int		curr_col;
	int		n_lines;
	int		n_cols;
	Col		*cols;

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
	Size			len;

	bool			linkok;
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
	char *identifier;
	char *color;
} Color;

typedef struct {
	Arg		*args;
	Flag	flags;
	File	**file_system;

	int		size;
	int		nb_file;
	bool	displayed;

	int		return_status;
	int		level;
	int		cols;
	bool	def;

	Color	*colors;
	int		n_colors;
	bool	error_colors;
} Command;


/* === UTILS === */
void	free_file(File *file, bool long_display);
void	free_childs(File *file, bool long_display);
int		free_command(Command *cmd);
void	free_colors(Command *cmd);

/* === STATS === */
char	*clean_join(char *origin, const char *to_join);
int		analyze_file(File *file, bool long_display);

/* === DISPLAY === */
char	*link_color(Command *cmd, File *file);
char	*color(Command *cmd, File *file);
int		round_split(int a, int b);
void	display(Command *cmd, File *node);
void	display_file(Command *cmd, File *file, Size *size, bool last);
void	announce_path(Command *cmd, File *node);
void	calculate_size(Size *size, File *node);

/* === PARSING === */
void	get_flags(Command *cmd);
bool	fatal_error(Command *cmd);
Command	*init_cmd(int ac, char **av, char **env);

/* === EXECUTION === */
int		add_to_file_system(File *parent, struct dirent *entry, bool long_display);
void	ft_ls(Command *cmd, File *parent);

/* === SORT === */
void	sort(Command *cmd, File **arr, int size);
