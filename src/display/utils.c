#include "header.h"

static void put_spaces(char *str, int max_size, int current_size) {
	while (current_size++ < max_size)
		ft_putchar_fd(' ', 1);
	ft_printf("%s ", str);
}

static void quoted(Command *cmd, char *str) {
	if (cmd->flags & quotes)
		ft_putstr_fd("\"", 1);
	ft_putstr_fd(str, 1);
	if (cmd->flags & quotes)
		ft_putstr_fd("\"", 1);
}

static void display_file_name(Command *cmd, File *file) {
	ft_putstr_fd(COLOR(file->type), 1);
	quoted(cmd, file->name);
	ft_putstr_fd(RESET, 1);
}

void display_file(Command *cmd, File *node, Size *size, bool last) {
	if (cmd->flags & long_display) {
		if (node->error == STAT) {
			ft_printf("%c???????? ? ? ? ?            ? ", node->type == DIRECTORY ? 'd' : '-');
			display_file_name(cmd, node);
			ft_putchar_fd('\n', 1);
			return ;
		}
		ft_printf("%s ", node->permissions);
		put_spaces(node->nb_links, size->link, ft_strlen(node->nb_links));
		
		if (!(cmd->flags & no_owner))
			put_spaces(node->owner, size->owner, ft_strlen(node->owner));

		put_spaces(node->group, size->group, ft_strlen(node->group));
		put_spaces(node->size, size->size, ft_strlen(node->size));
		ft_printf("%s ", node->last_modif_str);
		display_file_name(cmd, node);

		if (node->type == SYMLINK || node->type == DEAD_LINK) {
			ft_putstr_fd(" -> ", 1);
			ft_putstr_fd(COLOR(node->link_type), 1);
			quoted(cmd, node->link_to);
			ft_putstr_fd(RESET, 1);
		}
		ft_putchar_fd('\n', 1);
	} else {
		display_file_name(cmd, node);
		ft_putstr_fd(last ? "\n" : cmd->flags & commas ? ", " : "  ", 1);
	}
}
