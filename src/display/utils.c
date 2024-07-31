#include "header.h"

static QuoteMode quote_mode(Command *cmd, char *name) {
	if (cmd->flags & quotes)
		return DOUBLE_QUOTE;
	else if (cmd->flags & literal)
		return NO_QUOTE;
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

void quoted(Command *cmd, char *str) {
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

static void display_file_name(Command *cmd, File *file, char *permissions) {
	if (!cmd->def && !cmd->error_colors) {
		ft_printf("\e[%sm", color(cmd, file, permissions));
	}
	quoted(cmd, NAME(file));
	if (!cmd->def && !cmd->error_colors)
		ft_putstr_fd(RESET, 1);
}

static void display_link_name(Command *cmd, File *file) {
	ft_putstr_fd(" -> ", 1);
	if (!cmd->def && !cmd->error_colors)
		ft_printf("\e[%sm", (link_color(cmd, file)));
	quoted(cmd, file->link_to);
	if (!cmd->def && !cmd->error_colors)
		ft_putstr_fd(RESET, 1);
}

void long_file_display(Command *cmd, File *file, Padding *padding, char *permissions) {
	if (file->error == STAT) {
		ft_printf("%c???????? ? ? ? ?            ? ", file->type == DIRECTORY ? 'd' : '-');
		display_file_name(cmd, file, permissions);
		ft_putchar_fd('\n', 1);
		return ;
	} else if (file->error)
		return ;

	ft_printf("%s", permissions);
	ft_printf("%s ", file->has_acl ? "+" : padding->one_got_ext ? " " : "");
	put_spaces_nbr(file->nb_links, padding->link, false);

	if (!(cmd->flags & no_owner))
		put_spaces_str(file->owner, padding->owner, ft_strlen(file->owner));
	put_spaces_str(file->group, padding->group, ft_strlen(file->group));

	if (file->type != CHARACTER) {
		put_spaces_nbr(file->size, padding->size_minor + padding->major + (padding->major ? 2 : 0), false);
	}
	else {
		put_spaces_nbr(file->major, padding->major, true);
		put_spaces_nbr(file->minor, padding->size_minor, false);
	}

	char time[13] = "";
	ft_strlcpy(time, ctime(&file->last_modif) + 4, 13);
	ft_printf("%s ", time);

	display_file_name(cmd, file, permissions);
	if (file->type == SYMLINK)
		display_link_name(cmd, file);

	ft_putchar_fd('\n', 1);
}

void display_file(Command *cmd, File *file, Padding *padding, bool last) {
	char permissions[11] = "";
	check_permissions(file, file->mode, permissions);

	if (cmd->flags & long_display) {
		long_file_display(cmd, file, padding, permissions);
	} else {
		display_file_name(cmd, file, permissions);
		if (cmd->flags & commas && !last)
			ft_putchar_fd(',', 1);
		if (!last) {
			if (!cmd->def)
				put_spaces_str(cmd->flags & commas ? "" : " ", padding->cols[padding->curr_col].size, ft_strlen(NAME(file)));
			else
				ft_printf(cmd->flags & commas ? ",\n" : "\n");
		}
	}
}
