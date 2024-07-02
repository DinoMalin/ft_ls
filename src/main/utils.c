#include "header.h"

int free_command(Command *cmd) {
	int return_status = cmd->return_status;

	for (int i = 0; i < cmd->size; i++) {
		free(cmd->args[i].content);
	}
	for (int i = 0; i < cmd->nb_file; i++) {
		free_file(cmd->file_system[i], cmd->flags & long_display);
	}
	free(cmd->args);
	free(cmd->file_system);
	if (cmd->flags & colors)
		free_colors(cmd);
	free(cmd);
	return return_status;
}

void free_childs(File *parent, bool long_display) {
	for (int i = 0; i < parent->nb_childs; i++) {
		free_file(parent->childs[i], long_display);
	}
}

void free_file(File *file, bool long_display) {
	free(file->name);
	free(file->path);
	free(file->childs);
	if (long_display) {
		free(file->owner);
		free(file->group);
		free(file->nb_links);
		free(file->size);
	}
	if (file->type == CHARACTER) {
		free(file->major);
		free(file->minor);
	}
	free(file);
}

char *clean_join(char *origin, const char *to_join) {
	char *tmp = origin;
	char *res = ft_strjoin(origin, to_join);
	free(tmp);
	return res;
}

void free_colors(Command *cmd) {
	for (int i = 0; i < cmd->n_colors; i++) {
		free(cmd->colors[i].identifier);
		free(cmd->colors[i].color);
	}
	free(cmd->colors);
}