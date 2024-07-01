#include "header.h"

static char *get_link_path(File *link) {
	int len = ft_strlen(link->path) - ft_strlen(link->name);
	char *result = malloc(len + 1);

	result[len] = '\0';
	for (int i = 0; i < len; i++) {
		result[i] = link->path[i];
	}

	result = clean_join(result, link->link_to);
	return result;
}

static void	add_file_to_link(File *link) {
	struct stat statbuf;
	char *link_path = get_link_path(link);

	if (lstat(link_path, &statbuf) == -1) {
		free(link_path);
		return ;
	}

	link->linkok = true;
	if (S_ISDIR(statbuf.st_mode))
		link->link_type = DIRECTORY;
	else if (S_ISREG(statbuf.st_mode))
		link->link_type = REGULAR_FILE;
	else if (S_ISCHR(statbuf.st_mode))
		link->link_type = CHARACTER;
	else if (S_ISBLK(statbuf.st_mode))
		link->link_type = BLOCK;
	else if (S_ISFIFO(statbuf.st_mode))
		link->link_type = PIPE;
	else if (S_ISSOCK(statbuf.st_mode))
		link->link_type = SOCKET;
	free(link_path);
}

static void permissions(File *node, mode_t mode) {
	node->permissions[0] =	node->type == DIRECTORY	? 'd'
						:	node->type == CHARACTER	? 'c'
						:	node->type == SYMLINK	? 'l'
						:	node->type == BLOCK		? 'b'
						:	node->type == PIPE		? 'p'
						:	node->type == SOCKET	? 's'
						: '-';
	node->permissions[1] = mode & S_IRUSR ? 'r' : '-';
	node->permissions[2] = mode & S_IWUSR ? 'w' : '-';
	node->permissions[3] = mode & S_IXUSR ? 'x' : '-';
	node->permissions[4] = mode & S_IRGRP ? 'r' : '-';
	node->permissions[5] = mode & S_IWGRP ? 'w' : '-';
	node->permissions[6] = mode & S_IXGRP ? 'x' : '-';
	node->permissions[7] = mode & S_IROTH ? 'r' : '-';
	node->permissions[8] = mode & S_IWOTH ? 'w' : '-';
	node->permissions[9] = mode & S_ISVTX ? 't' : S_IXOTH ? 'x' : '-';
	node->permissions[10] = '\0';

	if (node->type == REGULAR_FILE && mode & S_IXUSR && mode & S_IXGRP && mode & S_IXOTH)
		node->type = EXECUTABLE;
}

int analyze_file(File *file, bool long_display) {
	struct stat statbuf = {};

	if (lstat(file->path, &statbuf) == -1) {
		file->error = STAT;
		ft_fprintf(2, ERNOAC, file->path);
		perror("");
		return 0;
	}

	if (S_ISDIR(statbuf.st_mode))
		file->type = DIRECTORY;
	else if (S_ISREG(statbuf.st_mode))
		file->type = REGULAR_FILE;
	else if (S_ISCHR(statbuf.st_mode))
		file->type = CHARACTER;
	else if (S_ISBLK(statbuf.st_mode))
		file->type = BLOCK;
	else if (S_ISFIFO(statbuf.st_mode))
		file->type = PIPE;
	else if (S_ISSOCK(statbuf.st_mode))
		file->type = SOCKET;
	else if (S_ISLNK(statbuf.st_mode)) {
		if (readlink(file->path, file->link_to, PATH_MAX) == -1) {
			perror("ft_ls");
			return 0;
		}
		add_file_to_link(file);
	}

	permissions(file, statbuf.st_mode);
	file->last_modif = statbuf.st_mtime;
	if (!long_display)
		return 1;
	ft_strlcpy(file->last_modif_str, ctime(&file->last_modif) + 4, 13);
	file->nb_links = ft_itoa(statbuf.st_nlink);

	struct passwd *pw;
	struct group *group;

	if ((pw = getpwuid(statbuf.st_uid)) == NULL) {
		perror("ft_ls");
		return 0;
	}

	if ((group = getgrgid(statbuf.st_gid)) == NULL) {
		perror("ft_ls");
		return 0;
	}

	file->owner = ft_strdup(pw->pw_name);
	file->group = ft_strdup(group->gr_name);
	file->size = ft_itoa(statbuf.st_size);
	file->blocks = statbuf.st_blocks;

	return 1;
}
