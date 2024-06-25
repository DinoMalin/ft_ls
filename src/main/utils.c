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
	free(cmd);
	return return_status;
}

void free_childs(File *node, bool long_display) {
	for (int i = 0; i < node->nb_childs; i++) {
		free_file(node->childs[i], long_display);
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
	free(file);
}

char *clean_join(char *origin, const char *to_join) {
	char *tmp = origin;
	char *res = ft_strjoin(origin, to_join);
	free(tmp);
	return res;
}