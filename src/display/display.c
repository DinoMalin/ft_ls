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
		for (int i = 0; i < node->size_childs.n_lines; i++) {
			for (int j = 0; i + j < node->nb_childs; j += node->size_childs.n_lines) {
				display_file(cmd, node->childs[i + j], &node->size_childs,
					(node->size_childs.curr_col + 1 >= node->size_childs.n_cols)
					|| (i + j + node->size_childs.n_lines >= node->nb_childs
					&& i + 1 >= node->size_childs.n_lines));
				if (++node->size_childs.curr_col >= node->size_childs.n_cols)
					node->size_childs.curr_col = 0;
			}
			ft_printf("\n");
		}
	}
}

void announce_path(Command *cmd, File *node) {
	if (!(cmd->flags & basic_display))
		return ;

	bool quoted = cmd->flags & quotes;
	ft_printf("%s%s%s:\n",
		quoted ? "\"" : "",
		node->path,
		quoted ? "\"" : ""
	);
}

static void get_cols_size(File *node) {
	for (int i = 0, col = 0, line = 0; i < node->nb_childs; i++, line++) {
		if (line >= node->size_childs.n_lines) {
			col++;
			line = 0;
		}
		int len = ft_strlen(node->childs[i]->name);
		if (node->size_childs.cols[col] < len)
			node->size_childs.cols[col] = len;
	}
}

void display(Command *cmd, File *node) {
	if (!handle_errors(node)) {
		free_file(node, cmd->flags & long_display);
		return ;
	}

	int total_len = node->size_childs.max_el * node->nb_childs;
	node->size_childs.n_lines = total_len / cmd->cols;
	if (!node->size_childs.n_lines)
		node->size_childs.n_lines = 1;
	node->size_childs.n_cols = node->nb_childs / node->size_childs.n_lines;
	if ((double)(node->nb_childs) / (double)(node->size_childs.n_lines) > node->size_childs.n_cols)
		node->size_childs.n_cols++;
	node->size_childs.cols = ft_calloc(node->size_childs.n_cols, sizeof(int));

	sort(node->childs, node->nb_childs,
		cmd->flags & time_modif ? compare_time : compare_name);

	get_cols_size(node);
	if (cmd->flags & long_display)
		ft_printf("total %d\n", node->total);
	
	list_files(cmd, node);
	free(node->size_childs.cols);
}
