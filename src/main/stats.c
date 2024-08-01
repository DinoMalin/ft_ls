#include "header.h"

static char *get_link_path(File *link) {
	char *result = ft_calloc((link->name + 1), sizeof(char));

	result[link->name] = '\0';
	for (int i = 0; i < link->name; i++) {
		result[i] = link->path[i];
	}

	result = clean_join(result, link->link_to);
	return result;
}

static void check_acl(File *file) {
	char buff[255];
	int size = listxattr(file->path, buff, 255);
	if (size <= 0) {
		return ;
	}

	char *attr = buff;
	while (attr < buff + size) {
		if (ft_strlen(attr)) {
			file->has_acl = true;
			return ;
		}
		attr += ft_strlen(attr) + 1;
	}
}

static void	add_file_to_link(File *link, struct stat parent_stats) {
	struct stat statbuf;
	char *link_path = get_link_path(link);

	if (lstat(link_path, &statbuf) == -1) {
		free(link_path);
		return ;
	}

	if (parent_stats.st_ino == statbuf.st_ino && parent_stats.st_dev == statbuf.st_dev) {
		free(link_path);
		link->mirrorlink = true;
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

static void get_link(File *file, struct stat statbuf, int size) {
	/* Some buggy file systems report garbage in st_size.  Defend
     against them by ignoring outlandish st_size values in the initial
     memory allocation.  */
	if (!size)
		size = SYMLINK_MAX;

	file->link_to = ft_calloc(size + 1, sizeof(char));
	if (readlink(file->path, file->link_to, size + 1) == -1) {
		ft_fprintf(2, ERSYMLINK, NAME(file), strerror(errno));
		file->rl_err = true;
	} else {
		add_file_to_link(file, statbuf);
	}
}

void check_permissions(File *node, mode_t mode, char *permissions) {
	permissions[0] =	node->type == DIRECTORY	? 'd'
						:	node->type == CHARACTER	? 'c'
						:	node->type == SYMLINK	? 'l'
						:	node->type == BLOCK		? 'b'
						:	node->type == PIPE		? 'p'
						:	node->type == SOCKET	? 's'
						: '-';
	permissions[1] = mode & S_IRUSR ? 'r' : '-';
	permissions[2] = mode & S_IWUSR ? 'w' : '-';
	permissions[3] = mode & S_IXUSR && node->type != CHARACTER && node->type != BLOCK ? (mode & S_ISUID ? 's' : 'x') : (mode & S_ISUID ? 'S' : '-');
	permissions[4] = mode & S_IRGRP ? 'r' : '-';
	permissions[5] = mode & S_IWGRP ? 'w' : '-';
	permissions[6] = mode & S_IXGRP && node->type != CHARACTER && node->type != BLOCK ? (mode & S_ISGID ? 's' : 'x') : (mode & S_ISGID ? 'S' : '-');
	permissions[7] = mode & S_IROTH ? 'r' : '-';
	permissions[8] = mode & S_IWOTH ? 'w' : '-';
	permissions[9] = mode & S_IXOTH && node->type != CHARACTER && node->type != BLOCK ? (mode & S_ISVTX ? 't' : 'x') : (mode & S_ISVTX ? 'T' : '-');
	permissions[10] = '\0';
}

int analyze_file(Command *cmd, File *file, bool long_display) {
	struct stat statbuf = {};

	if (lstat(file->path, &statbuf) == -1) {
		if (errno == EACCES)
			file->error = STAT;
		else
			file->error = NOSUCHFILE;
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
		file->type = SYMLINK;
		get_link(file, statbuf, statbuf.st_size);
	}

	file->mode = statbuf.st_mode;

	if (file->type == REGULAR_FILE
		&& file->mode & S_IXUSR
		&& file->mode & S_IXGRP
		&& file->mode & S_IXOTH)
		file->type = EXECUTABLE;

	check_acl(file);
	if (cmd->flags & access_time)
		file->last_modif = statbuf.st_atime;
	else
		file->last_modif = statbuf.st_mtime;

	if (!long_display)
		return 1;

	file->nb_links = statbuf.st_nlink;

	struct passwd	*pw;
	struct group	*group;

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
	file->size = statbuf.st_size;
	if (file->type == CHARACTER || file->type == BLOCK) {
		file->major = major(statbuf.st_rdev);
		file->minor = minor(statbuf.st_rdev);
	}
	file->blocks = statbuf.st_blocks;

	return 1;
}
