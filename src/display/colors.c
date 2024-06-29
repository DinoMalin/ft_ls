#include "header.h"

bool ends_with(char *str, char *end) {
	return !ft_strcmp(str + ft_strlen(str) - ft_strlen(end), end);
}

char *color(Command *cmd, File *file) {
	for (int i = 0; i < cmd->n_colors; i++) {
		if (!ft_strcmp(cmd->colors[i].identifier, "di") && file->type == DIRECTORY)
			return cmd->colors[i].color;
		else if (!ft_strcmp(cmd->colors[i].identifier, "ln") && file->type == SYMLINK)
			return cmd->colors[i].color;
		else if (!ft_strcmp(cmd->colors[i].identifier, "ex") && file->type == EXECUTABLE)
			return cmd->colors[i].color;
		else if (!ft_strcmp(cmd->colors[i].identifier, "or") && file->type == ORPHAN_LINK)
			return cmd->colors[i].color;
		else if (cmd->colors[i].identifier[0] == '*' && ends_with(file->name, cmd->colors[i].identifier + 1))
			return cmd->colors[i].color;
	}
	return "";
}