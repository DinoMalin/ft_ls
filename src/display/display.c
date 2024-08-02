#include "header.h"

int handle_errors(File *node) {
	if (node->type == REGULAR_FILE || node->error)
		return 0;
	else if (node->mirrorlink) {
		ft_fprintf(2, ERTOOMANYLNK, NAME(node));
		return 0;
	}
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

void default_list(Command *cmd, File *node, Padding *padding) {
	for (int i = 0; i < node->nb_childs; i++) {
		display_file(cmd, node->childs[i], padding, i >= node->nb_childs);
	}
}

static void list_files(Command *cmd, File *node, Padding *padding) {
	for (int line = 0; line < padding->n_lines; line++) {
		for (padding->curr_col = 0; padding->curr_col < padding->n_cols && padding->cols[padding->curr_col].files[line] != -1; padding->curr_col++) {
			display_file(cmd, node->childs[padding->cols[padding->curr_col].files[line]], padding,
				padding->curr_col >= padding->n_cols - 1 || line + padding->curr_col >= node->nb_childs - 1);
		}
		ft_printf("\n");
	}
	for (int col = 0; col < padding->n_cols; col++) {
		free(padding->cols[col].files);
	}
}

void announce_path(Command *cmd, File *node) {
	if (!(cmd->flags & basic_display))
		return ;

	quoted(cmd, node->path);
	ft_printf(":\n");
}

static void get_cols_indexes(File *node, Padding *padding) {
	for (int i = 0, col = 0, line = 0; i < node->nb_childs; i++, line++) {
		if (line >= padding->n_lines) {
			line = 0;
			col++;
		}
		if (!line) {
			padding->cols[col].files = ft_calloc(padding->n_lines, sizeof(int));
			for (int k = 0; k < padding->n_lines; k++) {
				padding->cols[col].files[k] = -1;
			}
		}
		padding->cols[col].files[line] = i;

		int len = ft_strlen(NAME(node->childs[i]));
		if (padding->cols[col].size < len)
			padding->cols[col].size = len;
	}
}

void display(Command *cmd, File *node) {
	if (!handle_errors(node)) {
		free_file(node, cmd->flags & long_display);
		return ;
	}
	if (cmd->flags & long_display)
		ft_printf("total %d\n", node->total);
	if (!node->nb_childs)
		return ;

	Padding padding = {};
	
	get_padding(cmd, node, &padding);
	sort(cmd, node->childs, node->nb_childs);

	if (cmd->def || cmd->flags & long_display)
		default_list(cmd, node, &padding);
	else {
		get_cols_indexes(node, &padding);
		list_files(cmd, node, &padding);
	}
	free_padding(&padding);
}
