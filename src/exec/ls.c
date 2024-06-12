#include "header.h"

char *clean_join(char *origin, const char *to_join) {
	char *tmp = origin;
	char *res = ft_strjoin(origin, to_join);
	free(tmp);
	return res;
}

void pre_treatment(Arg *arg, Command *cmd) {
	DIR	*dir;

	if (!(dir = opendir(arg->content))) {
		if (ft_strcmp(strerror(errno), "Permission denied")) {
			if (ft_strlen(cmd->perm_errors))
				clean_join(cmd->perm_errors, "\n");
			cmd->perm_errors = clean_join(cmd->perm_errors, "ft_ls : cannot access '");
			cmd->perm_errors = clean_join(cmd->perm_errors, arg->content);
			cmd->perm_errors = clean_join(cmd->perm_errors, "': Permission denied");
			arg->error.importance = 2;
		} else {
			perror(ERNOAC);
			arg->error.importance = 2;
		}
	}
}

int	ft_ls(const char *path, Command *cmd) {
	DIR				*dir;
	struct dirent	*entry;

	(void)cmd;
	if (!(dir = opendir(path))) {
		ft_printf(ERNOAC, path);
		perror("");
		return 2;
	}

	while ((entry = readdir(dir))) {
		if (!(cmd->flags & all)
			&& (!ft_strcmp(entry->d_name, ".") || !ft_strcmp(entry->d_name, "..")))
			continue;
		ft_printf("%s ", entry->d_name);
	}

	closedir(dir);
	return 0;
}
