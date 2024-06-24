#include "header.h"

int handle_errors(File *node) {
	if (node->type == REGULAR_FILE)
		return 0;
	if (node->error && node->error == NOSUCHFILE)
		return 0;
	else if (node->error && node->error == NOPERM) {
		ft_fprintf(2, ERNOPERM, node->path);
		return 0;
	}
	return 1;
}

void put_spaces(int max_size, int current_size) {
	while (current_size++ < max_size)
		ft_printf(" ");
}

void display_file(Command *cmd, File *node, Size *size, bool last) {
	if (cmd->flags & long_display) {
		ft_printf("%s ", node->permissions);
		put_spaces(size->link, ft_strlen(node->nb_links));
		ft_printf("%s ", node->nb_links);
		if (!(cmd->flags & no_owner)) {
			put_spaces(size->owner, ft_strlen(node->owner));
			ft_printf("%s ", node->owner);
		}
		put_spaces(size->group, ft_strlen(node->group));
		ft_printf("%s ", node->group);
		put_spaces(size->size, ft_strlen(node->size));
		ft_printf("%s ", node->size);
		ft_printf("%s ", node->last_modif_str);
		ft_putstr_fd(COLOR(node->type), 1);
		if (cmd->flags & quotes)
			ft_putstr_fd("\"", 1);
		ft_putstr_fd(node->name, 1);
		if (cmd->flags & quotes)
			ft_putstr_fd("\"", 1);
		ft_putstr_fd(RESET, 1);
		if (node->type == SYMLINK || node->type == DEAD_LINK) {
			ft_putstr_fd(" -> ", 1);
			ft_putstr_fd(COLOR(node->link_type), 1);
			ft_putstr_fd(node->link_to, 1);
			ft_putstr_fd(RESET, 1);
		}
		ft_putchar_fd('\n', 1);
	} else {
		ft_putstr_fd(COLOR(node->type), 1);
		if (cmd->flags & quotes)
			ft_putstr_fd("\"", 1);
		ft_putstr_fd(node->name, 1);
		if (cmd->flags & quotes)
			ft_putstr_fd("\"", 1);
		ft_putstr_fd(RESET, 1);
		if (cmd->flags & commas && !last)
			ft_putstr_fd(",", 1);
		if (!last)
			ft_putstr_fd("  ", 1);
		if (last)
			ft_putstr_fd("\n", 1);
	}
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
