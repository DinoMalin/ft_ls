#include "header.h"

int free_command(Command *cmd) {
	int return_status = cmd->return_status;

	for (int i = 0; i < cmd->size; i++) {
		free(cmd->args[i].content);
	}
	free(cmd->args);
	free(cmd->file_system);
	free(cmd);
	return return_status;
}

void free_file(File *file) {
	if (file->error)
		free(file->error);
	free(file->name);
	free(file->path);
	free(file->childs);
	free(file->owner);
	free(file->group);
	free(file);

}

char *clean_join(char *origin, const char *to_join) {
	char *tmp = origin;
	char *res = ft_strjoin(origin, to_join);
	free(tmp);
	return res;
}

void permissions(File *file, mode_t mode) {
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
}

void analyze_file(File *file) {
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

	file->last_modif = statbuf.st_mtime;
	ft_strlcpy(file->last_modif_str, ctime(&file->last_modif) + 4, 13);
	permissions(file, statbuf.st_mode);
	file->nb_links = statbuf.st_nlink;
	file->blocks = statbuf.st_blocks;

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
	file->size = statbuf.st_size;
}
