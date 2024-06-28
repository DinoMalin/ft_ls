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

void default_list(Command *cmd, File *node) {
	if (cmd->flags & reverse) {
		for (int i = node->nb_childs - 1; i >= 0; i--) {
			display_file(cmd, node->childs[i], &node->len, i == 0);
		}
	} else {
		for (int i = 0; i < node->nb_childs; i++) {
			display_file(cmd, node->childs[i], &node->len, i >= node->nb_childs);
		}
	}
}

void list_files(Command *cmd, File *node) {
	if (cmd->flags & reverse) {
		for (int i = node->nb_childs - 1; i >= 0; i--) {
			display_file(cmd, node->childs[i], &node->len, i == 0);
		}
	} else {
		for (int line = 0; line < node->len.n_lines; line++) {
			for (node->len.curr_col = 0; node->len.curr_col < node->len.n_cols && line + node->len.curr_col < node->nb_childs; node->len.curr_col++) {
				display_file(cmd, node->childs[node->len.cols[node->len.curr_col].files[line]], &node->len,
					node->len.curr_col >= node->len.n_cols - 1 || line + node->len.curr_col >= node->nb_childs - 1);
			}
			ft_printf("\n");
		}
		for (int col = 0; col < node->len.n_cols; col++) {
			free(node->len.cols[col].files);
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

static void get_cols_indexes(File *node) {
	for (int i = 0, col = 0, line = 0; i < node->nb_childs; i++, line++) {
		if (line >= node->len.n_lines) {
			line = 0;
			col++;
		}
		if (!line)
			node->len.cols[col].files = ft_calloc(node->len.n_lines, sizeof(int));
		node->len.cols[col].files[line] = i;

		int len = ft_strlen(node->childs[i]->name);
		if (node->len.cols[col].size < len)
			node->len.cols[col].size = len;
	}
}

void display(Command *cmd, File *node) {
	if (!handle_errors(node)) {
		free_file(node, cmd->flags & long_display);
		return ;
	}

	int total_len = node->len.max_el * node->nb_childs;

	node->len.n_lines = round_split(total_len, cmd->cols);
	if (!node->len.n_lines)
		node->len.n_lines = 1;
	node->len.n_cols = round_split(node->nb_childs, node->len.n_lines);

	node->len.cols = ft_calloc(node->len.n_cols, sizeof(Col));

	sort(node->childs, node->nb_childs,
		cmd->flags & time_modif ? compare_time : compare_name);

	get_cols_indexes(node);
	if (cmd->flags & long_display)
		ft_printf("total %d\n", node->total);
	
	list_files(cmd, node);
	free(node->len.cols);
}


/* Pour des raisons inconnues :
 - Segfault quand pipe dans cat -e ?????
 - LAST mal detecte apparemment, ce serait bien de variabiliser ou fonctionner tout ca :
	=> Pb avec colums entre 57 et 67 ????
*/