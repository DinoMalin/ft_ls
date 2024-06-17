#include "header.h"

int handle_errors(File *node) {
	if (node->type == REGULAR_FILE) {
		free_file(node);
		return 0;
	} if (node->error && !ft_strcmp(node->error, "ERNOSUCHFILE")) {
		free_file(node);
		return 0;
	} else if (node->error && !ft_strcmp(node->error, "ERNOPERM")) {
		ft_fprintf(2, ERNOPERM, node->path);
		return 0;
	}
	return 1;
}

void display_file(Command *cmd, File *node, int last) {
	if (cmd->flags & long_display) {
		ft_printf("%s %d %s %s %d %s %s%s%s%s%s%s%s%s%s",
			node->permissions,
			node->nb_links,
			cmd->flags & no_owner ? "" : node->owner,
			node->group,
			node->size,
			node->last_modif_str,
			COLOR(node->type),
			cmd->flags & quotes ? "\"" : "",
			node->name,
			cmd->flags & quotes ? "\"" : "",
			RESET,
			node->type == SYMLINK ? " -> " : "",
			node->type == SYMLINK ? COLOR(node->link_type) : "",
			node->type == SYMLINK ? node->link_to : "",
			node->type == SYMLINK ? RESET : ""
		);
		if (!last)
			ft_printf("\n");
	} else {
		ft_printf("%s%s%s%s%s%s ",
			COLOR(node->type),
			cmd->flags & quotes ? "\"" : "",
			node->name,
			cmd->flags & quotes ? "\"" : "",
			RESET,
			cmd->flags & commas && !last ? "," : ""
		);
	}
}

void recursive_display(Command *cmd, File *node) {
	if (cmd->flags & reverse) {
		for (int i = node->nb_childs - 1; i >= 0; i--) {
			if (node->childs[i]->type == DIRECTORY)
				display(cmd, node->childs[i]);
			else
				free_file(node->childs[i]);
		}
	} else {
		for (int i = 0; i < node->nb_childs; i++) {
			if (node->childs[i]->type == DIRECTORY)
				display(cmd, node->childs[i]);
			else
				free_file(node->childs[i]);
		}
	}
}

void display(Command *cmd, File *node) {
	if (!handle_errors(node))
		return ;

	!cmd->displayed ? cmd->displayed = true : ft_printf("\n");

	if (cmd->flags & basic_display)
		ft_printf("%s%s%s:\n",
			cmd->flags & quotes ? "\"" : "",
			node->path,
			cmd->flags & quotes ? "\"" : ""
		);

	sort(node->childs, node->nb_childs,
		cmd->flags & time_modif ? compare_time : compare_name);	

	if (cmd->flags & long_display)
		ft_printf("total %d\n", node->blocks);
	if (cmd->flags & reverse) {
		for (int i = node->nb_childs - 1; i >= 0; i--) {
			display_file(cmd, node->childs[i], i == 0);
		}
	} else {
		for (int i = 0; i < node->nb_childs; i++) {
			display_file(cmd, node->childs[i], i == node->nb_childs - 1);
		}
	}
	if (node->nb_childs)
		ft_printf("\n");
	if (cmd->flags & recursive)
		recursive_display(cmd, node);
	else {
		for (int i = 0; i < node->nb_childs; i++) {
			free_file(node->childs[i]);
		}
	}
	free_file(node);
}
