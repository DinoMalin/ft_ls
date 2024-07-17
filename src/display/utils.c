#include "header.h"

int round_split(int a, int b) {
	if (b == 0)
		return 0;
	int result = a / b;
	if ((double)(a) / (double)(b) > result)
		result++;
	return result;
}

static void put_spaces(char *str, int max_size, int current_size) {
	while (current_size++ < max_size) {
		ft_putchar_fd(' ', 1);
	}
	ft_printf("%s ", str);
}

static QuoteMode quote_mode(Command *cmd, char *name) {
	if (cmd->flags & quotes)
		return DOUBLE_QUOTE;
	for (int i = 0; name[i]; i++) {
		if (ft_strchr(" \\\"$&();*<>|[]~^`=", name[i]))
			return SINGLE_QUOTE;
	}
	if (ft_strlen(name) == 1 && ft_strchr("{}", name[0]))
		return SINGLE_QUOTE;
	if (ft_strchr(name, '\''))
		return DOUBLE_QUOTE;
	return NO_QUOTE;
}	

static void quoted(Command *cmd, char *str) {
	QuoteMode qm = quote_mode(cmd, str);

	if (qm == DOUBLE_QUOTE)
		ft_putstr_fd("\"", 1);
	else if (qm == SINGLE_QUOTE)
		ft_putstr_fd("\'", 1);

	ft_putstr_fd(str, 1);

	if (qm == DOUBLE_QUOTE)
		ft_putstr_fd("\"", 1);
	else if (qm == SINGLE_QUOTE)
		ft_putstr_fd("\'", 1);
}

static void display_file_name(Command *cmd, File *file) {
	if (!cmd->def && !cmd->error_colors) {
		ft_printf("\e[%sm", color(cmd, file));
	}
	quoted(cmd, file->name);
	if (!cmd->def && !cmd->error_colors)
		ft_putstr_fd(RESET, 1);
}

void display_file(Command *cmd, File *file, Size *size, bool last) {
	if (cmd->flags & long_display) {
		if (file->error == STAT) {
			ft_printf("%c???????? ? ? ? ?            ? ", file->type == DIRECTORY ? 'd' : '-');
			display_file_name(cmd, file);
			ft_putchar_fd('\n', 1);
			return ;
		}
		ft_printf("%s", file->permissions);
		ft_printf("%s ", file->has_ext ? file->has_acl ? "+" : "." : size->one_got_ext ? " " : "");
		put_spaces(file->nb_links, size->link, ft_strlen(file->nb_links));
		
		if (!(cmd->flags & no_owner))
			put_spaces(file->owner, size->owner, ft_strlen(file->owner));

		put_spaces(file->group, size->group, ft_strlen(file->group));

		if (file->type != CHARACTER)
			put_spaces(file->size, size->size_minor + size->major + 1, ft_strlen(file->size));
		else {
			put_spaces(file->major, size->major, ft_strlen(file->major));
			put_spaces(file->minor, size->size_minor, ft_strlen(file->minor));
		}

		ft_printf("%s ", file->last_modif_str);
		display_file_name(cmd, file);

		if (file->type == SYMLINK) {
			ft_putstr_fd(" -> ", 1);
			if (!cmd->def && !cmd->error_colors)
				ft_printf("\e[%sm", (link_color(cmd, file)));
			quoted(cmd, file->link_to);
			if (!cmd->def && !cmd->error_colors)
				ft_putstr_fd(RESET, 1);
		}
		ft_putchar_fd('\n', 1);
	} else {
		display_file_name(cmd, file);
		if (cmd->flags & commas && !last)
			ft_putchar_fd(',', 1);
		if (!last) {
			if (!cmd->def)
				put_spaces(cmd->flags & commas ? "" : " ", size->cols[size->curr_col].size, ft_strlen(file->name));
			else
				ft_printf(cmd->flags & commas ? ",\n" : "\n");
		}
	}
}
