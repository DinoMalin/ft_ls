#include "header.h"

int	ft_ls(const char *path) {
	DIR				*dir;
	struct dirent	*entry;

	if (!(dir = opendir(path))) {
		perror(ERNOAC);
		return 2;
	}

	while ((entry = readdir(dir))) {
		ft_printf("%s ", entry->d_name);
	}

	closedir(dir);
	return 0;
}
