#include "header.h"

int handle_errors(File *node) {
	if (node->type == REGULAR_FILE || node->error)
		return 0;
	return 1;
}

void recursive_display(Command *cmd, File *node) {
	if (cmd->flags & reverse) {
		for (int i = node->nb_childs - 1; i >= 0; i--) {
			if (node->childs[i]->type == DIRECTORY)
				display(cmd, node->childs[i]);
			else
				free_file(node->childs[i], cmd->flags & long_display);
		}
	} else {
		for (int i = 0; i < node->nb_childs; i++) {
			if (node->childs[i]->type == DIRECTORY)
				display(cmd, node->childs[i]);
			else
				free_file(node->childs[i], cmd->flags & long_display);
		}
	}
}

void list_files(Command *cmd, File *node) {
	if (cmd->flags & reverse) {
		for (int i = node->nb_childs - 1; i >= 0; i--) {
			display_file(cmd, node->childs[i], &node->size_childs, i == 0);
		}
	} else {
		for (int i = 0; i < node->nb_childs; i++) {
			display_file(cmd, node->childs[i], &node->size_childs, i == node->nb_childs - 1);
		}
	}
}

void announce_path(Command *cmd, File *node) {
	bool quoted = cmd->flags & quotes;
	if (cmd->flags & basic_display) {
		ft_printf("%s%s%s:\n",
			quoted ? "\"" : "",
			node->path,
			quoted ? "\"" : ""
		);
	}
}

void display(Command *cmd, File *node) {
	if (!handle_errors(node)) {
		free_file(node, cmd->flags & long_display);
		return ;
	}
	!cmd->displayed ? cmd->displayed = true : ft_printf("\n");

	announce_path(cmd, node);
	sort(node->childs, node->nb_childs,
		cmd->flags & time_modif ? compare_time : compare_name);

	if (cmd->flags & long_display)
		ft_printf("total %d\n", node->total);
	
	list_files(cmd, node);
}
