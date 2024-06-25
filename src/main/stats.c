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
		link->type = DEAD_LINK;
		link->link_type = DEAD_LINK;
		free(link_path);
		return ;
	}

	if (S_ISDIR(statbuf.st_mode))
		link->link_type = DIRECTORY;
	else if (S_ISREG(statbuf.st_mode))
		link->link_type = REGULAR_FILE;
	free(link_path);
}

static void permissions(File *file, mode_t mode) {
	file->permissions[0] = file->type == DIRECTORY ? 'd' : '-';
	file->permissions[1] = mode & S_IRUSR ? 'r' : '-';
	file->permissions[2] = mode & S_IWUSR ? 'w' : '-';
	file->permissions[3] = mode & S_IXUSR ? 'x' : '-';
	file->permissions[4] = mode & S_IRGRP ? 'r' : '-';
	file->permissions[5] = mode & S_IWGRP ? 'w' : '-';
	file->permissions[6] = mode & S_IXGRP ? 'x' : '-';
	file->permissions[7] = mode & S_IROTH ? 'r' : '-';
	file->permissions[8] = mode & S_IWOTH ? 'w' : '-';
	file->permissions[9] = mode & S_IXOTH ? 'x' : '-';
	file->permissions[10] = '\0';

	if (file->type == REGULAR_FILE && mode & S_IXUSR && mode & S_IXGRP && mode & S_IXOTH)
		file->type = EXECUTABLE;
}

void analyze_file(File *file, bool long_display) {
	struct stat statbuf;

	if (lstat(file->path, &statbuf) == -1)
		return ;

	if (S_ISDIR(statbuf.st_mode))
		file->type = DIRECTORY;
	else if (S_ISREG(statbuf.st_mode))
		file->type = REGULAR_FILE;
	else if (S_ISLNK(statbuf.st_mode)) {
		file->type = SYMLINK;
		if (readlink(file->path, file->link_to, PATH_MAX) == -1) {
			perror("ft_ls");
			return ;
		}
		add_file_to_link(file);
	}

	permissions(file, statbuf.st_mode);
	if (!long_display)
		return ;
	file->last_modif = statbuf.st_mtime;
	ft_strlcpy(file->last_modif_str, ctime(&file->last_modif) + 4, 13);
	file->nb_links = ft_itoa(statbuf.st_nlink);

	struct passwd *pw;
	struct group *group;

	if ((pw = getpwuid(statbuf.st_uid)) == NULL) {
		perror("ft_ls");
		return;
	}

	if ((group = getgrgid(statbuf.st_gid)) == NULL) {
		perror("ft_ls");
		return;
	}

	file->owner = ft_strdup(pw->pw_name);
	file->group = ft_strdup(group->gr_name);
	file->size = ft_itoa(statbuf.st_size);
	file->blocks = statbuf.st_blocks;
}
