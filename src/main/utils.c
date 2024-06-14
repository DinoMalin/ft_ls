#include "header.h"

char *clean_join(char *origin, const char *to_join) {
	char *tmp = origin;
	char *res = ft_strjoin(origin, to_join);
	free(tmp);
	return res;
}

void analyze_file(File *file) {
	struct stat statbuf;

	if (stat(file->path, &statbuf) == -1)
		return ;

	if (S_ISDIR(statbuf.st_mode))
		file->type = DIRECTORY;
	else if (S_ISLNK(statbuf.st_mode))
		file->type = SYMLINK;
	else if (S_ISREG(statbuf.st_mode))
		file->type = REGULAR_FILE;

	file->last_modif = statbuf.st_mtime;
}
