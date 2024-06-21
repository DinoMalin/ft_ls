#include "header.h"

void	ft_ls(Command *cmd, File *parent) {
	DIR				*dir;
	struct dirent	*entry;

	if (parent->type == REGULAR_FILE)
		return ;
	if (!(dir = opendir(parent->path))) {
		if (!ft_strcmp(strerror(errno), "Permission denied"))
			parent->error = NOPERM;
		else {
			ft_fprintf(2, ERNOAC, parent->path);
			perror("");
			parent->error = NOSUCHFILE;
		}
		cmd->return_status = 1;
		return ;
	}

	while ((entry = readdir(dir))) {
		if (!(cmd->flags & all) && (!ft_strcmp(entry->d_name, ".") || !ft_strcmp(entry->d_name, "..")))
			continue;
		if (!(cmd->flags & dotfiles) && entry->d_name[0] == '.')
			continue;
		add_to_file_system(parent, entry); 
	}

	closedir(dir);

	if (!(cmd->flags & dir_only))
		display(cmd, parent);
	if (cmd->flags & recursive) {
		for (int i = 0; i < parent->nb_childs; i++) {
			if (parent->childs[i]->type == DIRECTORY
				&& ft_strcmp(parent->childs[i]->name, ".")
				&& ft_strcmp(parent->childs[i]->name, ".."))
				ft_ls(cmd, parent->childs[i]);
		}
	}
	free_childs(parent);
}
