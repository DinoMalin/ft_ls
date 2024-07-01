#include "header.h"

bool ends_with(char *str, char *end) {
	int len_str = ft_strlen(str);
	int len_end = ft_strlen(end);
	if (len_end > len_str)
		return false;
	return !ft_strcmp(str + len_str - len_end, end);
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
		else if (!ft_strcmp(cmd->colors[i].identifier, "cd") && file->type == CHARACTER)
			return cmd->colors[i].color;
		else if (cmd->colors[i].identifier[0] == '*' && ends_with(file->name, cmd->colors[i].identifier + 1))
			return cmd->colors[i].color;
	}
	return "";
}

char *link_color(Command *cmd, File *file) {
	for (int i = 0; i < cmd->n_colors; i++) {
		if (!ft_strcmp(cmd->colors[i].identifier, "di") && file->link_type == DIRECTORY)
			return cmd->colors[i].color;
		else if (!ft_strcmp(cmd->colors[i].identifier, "ln") && file->link_type == SYMLINK)
			return cmd->colors[i].color;
		else if (!ft_strcmp(cmd->colors[i].identifier, "ex") && file->link_type == EXECUTABLE)
			return cmd->colors[i].color;
		else if (!ft_strcmp(cmd->colors[i].identifier, "or") && file->link_type == ORPHAN_LINK)
			return cmd->colors[i].color;
		else if (!ft_strcmp(cmd->colors[i].identifier, "cd") && file->type == CHARACTER)
			return cmd->colors[i].color;
		else if (cmd->colors[i].identifier[0] == '*' && ends_with(file->link_to, cmd->colors[i].identifier + 1))
			return cmd->colors[i].color;
	}
	return "";
}