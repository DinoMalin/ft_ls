#include "header.h"

void	ft_ls(Command *cmd, File *parent) {
	DIR				*dir;
	struct dirent	*entry;

	if (parent->type != DIRECTORY && parent->type != SYMLINK)
		return ;
	if (!(dir = opendir(parent->path))) {
		ft_fprintf(2, ERNOPERM, parent->path);
		perror("");
		cmd->return_status = 1;
		return ;
	}

	!cmd->displayed ? cmd->displayed = true : ft_printf("\n");
	announce_path(cmd, parent);

	while ((entry = readdir(dir))) {
		if (!(cmd->flags & all) && (!ft_strcmp(entry->d_name, ".") || !ft_strcmp(entry->d_name, "..")))
			continue;
		if (!(cmd->flags & dotfiles) && entry->d_name[0] == '.')
			continue;
		if (!add_to_file_system(cmd, parent, entry, cmd->flags & long_display))
			cmd->return_status = 1;
	}

	closedir(dir);

	if (!(cmd->flags & dir_only))
		display(cmd, parent);
	if (cmd->flags & recursive) {
		for (int i = 0; i < parent->nb_childs; i++) {
			if (parent->childs[i]->type == DIRECTORY
				&& ft_strcmp(NAME(parent->childs[i]), ".")
				&& ft_strcmp(NAME(parent->childs[i]), "..")) {
					ft_ls(cmd, parent->childs[i]);
				}
		}
	}
	free_childs(parent, cmd->flags & long_display);
}
