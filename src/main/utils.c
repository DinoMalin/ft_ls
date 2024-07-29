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
	if (file->link_to)
		free(file->link_to);
	free(file);
}

void free_padding(Padding *padding) {
	if (padding->cols)
		free(padding->cols);
}

char *join_with_separator(char *str1, char *str2, char sep) {
	if (!str1 || !str2)
		return NULL;

	int len_s1 = ft_strlen(str1);
	int len_s2 = ft_strlen(str2);
	int len =  len_s1 + len_s2 + 1;

	char *result = malloc((len + 1) * sizeof(char));
	if (!result)
		return NULL;
	for (int i = 0; i < len_s1; i++) {
		result[i] = str1[i];
	}
	result[len_s1] = sep;
	for (int i = 0; i < len_s2; i++) {
		result[len_s1 + i + 1] = str2[i];
	}
	result[len] = '\0';
	return result;
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
