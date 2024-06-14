#include "header.h"

void free_command(Command *cmd) {
	for (int i = 0; i < cmd->size; i++) {
		free(cmd->args[i].content);
	}
	free(cmd->args);
	free(cmd->perm_errors);
}

void display(Command *cmd, File *node) {
	if (node->type == REGULAR_FILE)
		return ;
	if (node->error && !ft_strcmp(node->error, "ERNOSUCHFILE"))
		return;
	else if (node->error && node->level && !ft_strcmp(node->error, "ERNOPERM")) {
		ft_fprintf(2, ERNOPERM, node->path);
		return;
	}

	if (!cmd->displayed)
		cmd->displayed = true;
	else
		ft_printf("\n");

	if (cmd->flags & basic_display)
		ft_printf("%s:\n", node->path);

	sort(node, compare_name);

	if (cmd->flags & reverse) {
		for (int i = node->nb_childs - 1; i >= 0; i--) {
			if (node->childs[i]->type == DIRECTORY)
				ft_printf(DIR_COLOR);
			ft_printf("%s%s ", node->childs[i]->name, RESET);
		}
		ft_printf("\n");
		if (!(cmd->flags & recursive))
			return ;
		for (int i = node->nb_childs - 1; i >= 0; i--) {
			if (node->childs[i]->type == DIRECTORY)
				display(cmd, node->childs[i]);
		}
	} else {
		for (int i = 0; i < node->nb_childs; i++) {
			if (node->childs[i]->type == DIRECTORY)
				ft_printf(DIR_COLOR);
			ft_printf("%s%s ", node->childs[i]->name, RESET);
		}
		ft_printf("\n");
		if (!(cmd->flags & recursive))
			return ;
		for (int i = 0; i < node->nb_childs; i++) {
			if (node->childs[i]->type == DIRECTORY)
				display(cmd, node->childs[i]);
		}	
	}
}

int main(int ac, char **av) {
	Command *cmd = init_cmd(ac, av);

	if (fatal_error(cmd)) {
		free_command(cmd);
		return 2;
	}

	for (int i = 0; i < cmd->nb_file; i++) {
		ft_ls(cmd, cmd->file_system[i]);
	}

	bool files_in_args = false;
	for (int i = 0; i < cmd->nb_file; i++) {
		if (files_in_args)
			ft_printf(" ");
		if (cmd->file_system[i]->type == REGULAR_FILE) {
			ft_printf("%s", cmd->file_system[i]->path);
			files_in_args = true;
		}
	}

	if (files_in_args)
		ft_printf("\n\n");

	for (int i = 0; i < cmd->nb_file; i++) {
		display(cmd, cmd->file_system[i]);
	}

	for (int i = 0; i < cmd->nb_file; i++) {
		if (cmd->file_system[i]->error && !ft_strcmp(cmd->file_system[i]->error, "ERNOPERM"))
			ft_fprintf(2, ERNOPERM, cmd->file_system[i]->path);
	}

	free_command(cmd);

	return 0;
}
