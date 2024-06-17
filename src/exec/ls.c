#include "header.h"

void	ft_ls(Command *cmd, File *parent) {
	DIR				*dir;
	struct dirent	*entry;

	if (parent->type == REGULAR_FILE)
		return ;
	if (!(dir = opendir(parent->path))) {
		if (!ft_strcmp(strerror(errno), "Permission denied"))
			parent->error = ft_strdup("ERNOPERM");
		else {
			ft_fprintf(2, ERNOAC, parent->path);
			perror("");
			parent->error = ft_strdup("ERNOSUCHFILE");
		}
		cmd->return_status = 1;
		return ;
	}

	while ((entry = readdir(dir))) {
		if (!(cmd->flags & all)
			&& entry->d_name[0] == '.')
			continue;
		add_to_file_system(parent, entry); 
	}

	closedir(dir);

	if (!(cmd->flags & recursive))
		return ;
	for (int i = 0; i < parent->nb_childs; i++) {
		if (parent->childs[i]->type == DIRECTORY
			&& ft_strcmp(parent->childs[i]->name, ".")
			&& ft_strcmp(parent->childs[i]->name, ".."))
			ft_ls(cmd, parent->childs[i]);
	}
}
