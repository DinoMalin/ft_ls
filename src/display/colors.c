#include "header.h"

bool ends_with(char *str, char *end) {
	int len_str = ft_strlen(str);
	int len_end = ft_strlen(end);
	if (len_end > len_str)
		return false;
	return !ft_strcmp(str + len_str - len_end, end);
}

char *color(Command *cmd, File *file, char *permissions) {
	for (int i = 0; i < cmd->n_colors; i++) {
		if (!ft_strcmp(cmd->colors[i].identifier, "di") && file->type == DIRECTORY && permissions[8] != 'w' && permissions[9] != 't' && permissions[9] != 'T')
			return cmd->colors[i].color;
		else if (!ft_strcmp(cmd->colors[i].identifier, "tw") && file->type == DIRECTORY && permissions[8] == 'w' && (permissions[9] == 't' || permissions[9] == 'T'))
			return cmd->colors[i].color;
		else if (!ft_strcmp(cmd->colors[i].identifier, "st") && file->type == DIRECTORY && permissions[8] != 'w' && (permissions[9] == 't' || permissions[9] == 'T'))
			return cmd->colors[i].color;
		else if (!ft_strcmp(cmd->colors[i].identifier, "ow") && file->type == DIRECTORY && permissions[8] == 'w' && permissions[9] != 't' && permissions[9] != 'T')
			return cmd->colors[i].color;
		else if (!ft_strcmp(cmd->colors[i].identifier, "ln") && file->type == SYMLINK && file->linkok)
			return cmd->colors[i].color;
		else if (!ft_strcmp(cmd->colors[i].identifier, "ex") && file->type == EXECUTABLE)
			return cmd->colors[i].color;
		else if (!ft_strcmp(cmd->colors[i].identifier, "su") && (permissions[3] == 's' || permissions[3] == 'S'))
			return cmd->colors[i].color;
		else if (!ft_strcmp(cmd->colors[i].identifier, "sg") && (permissions[6] == 's' || permissions[6] == 'S'))
			return cmd->colors[i].color;
		else if (!ft_strcmp(cmd->colors[i].identifier, "or") && file->type == SYMLINK && !file->linkok)
			return cmd->colors[i].color;
		else if (!ft_strcmp(cmd->colors[i].identifier, "cd") && file->type == CHARACTER)
			return cmd->colors[i].color;
		else if (!ft_strcmp(cmd->colors[i].identifier, "bd") && file->type == BLOCK)
			return cmd->colors[i].color;
		else if (!ft_strcmp(cmd->colors[i].identifier, "so") && file->type == SOCKET)
			return cmd->colors[i].color;
		else if (!ft_strcmp(cmd->colors[i].identifier, "pi") && file->type == PIPE)
			return cmd->colors[i].color;
		else if (cmd->colors[i].identifier[0] == '*' && ends_with(NAME(file), cmd->colors[i].identifier + 1))
			return cmd->colors[i].color;
	}
	return "";
}

char *link_color(Command *cmd, File *file) {
	for (int i = 0; i < cmd->n_colors; i++) {
		if (!ft_strcmp(cmd->colors[i].identifier, "or") && !file->linkok)
			return cmd->colors[i].color;
		else if (!file->linkok)
			continue;
		else if (!ft_strcmp(cmd->colors[i].identifier, "di") && file->link_type == DIRECTORY)
			return cmd->colors[i].color;
		else if (!ft_strcmp(cmd->colors[i].identifier, "ln") && file->link_type == SYMLINK)
			return cmd->colors[i].color;
		else if (!ft_strcmp(cmd->colors[i].identifier, "ex") && file->link_type == EXECUTABLE)
			return cmd->colors[i].color;
		else if (!ft_strcmp(cmd->colors[i].identifier, "cd") && file->type == CHARACTER)
			return cmd->colors[i].color;
		else if (!ft_strcmp(cmd->colors[i].identifier, "bd") && file->type == CHARACTER)
			return cmd->colors[i].color;
		else if (cmd->colors[i].identifier[0] == '*' && ends_with(file->link_to, cmd->colors[i].identifier + 1))
			return cmd->colors[i].color;
	}
	return "";
}