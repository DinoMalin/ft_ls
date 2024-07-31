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
#include <limits.h>
#include <locale.h>
#include <sys/ioctl.h>

#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/xattr.h>

#define RESET		"\e[0m"
#define NAME(file)	file->path + file->name

typedef enum {
	NOERROR,
	NOSUCHFILE,
	NOPERM,
	STAT,
} Error;

typedef enum {
	OPTION		= 1 << 0,
	LONG_OPTION	= 1 << 1,
	END_OPTION	= 1 << 1,
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
	long_display		= 1 << 0,
	recursive			= 1 << 1,
	all					= 1 << 2,
	reverse				= 1 << 3,
	time_sort			= 1 << 4,
	access_time			= 1 << 5,
	help				= 1 << 6,
	basic_display		= 1 << 7,
	commas				= 1 << 8,
	quotes				= 1 << 9,
	no_owner			= 1 << 10,
	dir_only			= 1 << 11,
	dotfiles			= 1 << 12,
	colors				= 1 << 13,
	literal				= 1 << 14,
	no_sort				= 1 << 15,
	version				= 1 << 16,
} Flag;

typedef enum {
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
} QuoteMode;

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
	int		curr_col;
	int		n_lines;
	int		n_cols;
	Col		*cols;

	size_t	link;
	size_t	owner;
	size_t	group;
	size_t	major;
	size_t	size_minor;
	bool	one_got_ext;
} Padding;

typedef struct File {
	FileType		type;
	struct File		**childs;
	Error			error;
	int				nb_childs;
	int				longest_el;

	bool			linkok;
	bool			mirrorlink;
	char			*link_to;
	FileType		link_type;

	char			*path;
	int				name;
	time_t			last_modif;
	mode_t			mode;
	int				nb_links;
	char			*owner;
	char			*group;
	int				size;
	int				major;
	int				minor;
	int				blocks;
	int				total;

	bool			has_acl;
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
void			free_file(File *file, bool long_display);
void			free_childs(File *file, bool long_display);
int				free_command(Command *cmd);
void			free_colors(Command *cmd);
void			free_padding(Padding *padding);
char			*join_with_separator(char *str1, char *str2, char sep);
unsigned int	get_size_nbr(int nb);

/* === STATS === */
char	*clean_join(char *origin, const char *to_join);
void	check_permissions(File *node, mode_t mode, char *permissions);
int		analyze_file(Command *cmd, File *file, bool long_display);

/* === DISPLAY === */
void	quoted(Command *cmd, char *str);
char	*link_color(Command *cmd, File *file);
char	*color(Command *cmd, File *file, char *permissions);
int		round_split(int a, int b);
void	display(Command *cmd, File *node);
void	display_file(Command *cmd, File *file, Padding *padding, bool last);
void	announce_path(Command *cmd, File *node);
void	calculate_padding(Padding *size, File *node);

void	put_spaces_str(char *str, int max_size, int curr_size);
void	put_spaces_nbr(int nbr, int max_size, bool commas);

/* === PARSING === */
void	get_flags(Command *cmd);
bool	fatal_error(Command *cmd);
Command	*init_cmd(int ac, char **av, char **env);

/* === EXECUTION === */
int		add_to_file_system(Command *cmd, File *parent, struct dirent *entry, bool long_display);
void	ft_ls(Command *cmd, File *parent);
void	get_padding(Command *cmd, File *parent, Padding *padding);

/* === SORT === */
void	sort(Command *cmd, File **arr, int size);
