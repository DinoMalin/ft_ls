#include "header.h"

void calculate_padding(Padding *padding, File *node) {
	if (!node->nb_links || !node->size || !node->owner || !node->group)
		return ;
	if (node->has_ext)
		padding->one_got_ext = true;
	if (ft_strlen(node->nb_links) > padding->link)
		padding->link = ft_strlen(node->nb_links);
	if (ft_strlen(node->size) > padding->size_minor)
		padding->size_minor = ft_strlen(node->size);
	if (ft_strlen(node->owner) > padding->owner)
		padding->owner = ft_strlen(node->owner);
	if (ft_strlen(node->group) > padding->group)
		padding->group = ft_strlen(node->group);
	if (node->type == CHARACTER && ft_strlen(node->major) > padding->major)
		padding->major = ft_strlen(node->major);
	if (node->type == CHARACTER && ft_strlen(node->minor) > padding->size_minor)
		padding->size_minor = ft_strlen(node->minor);
}

void get_padding(Command *cmd, File *parent, Padding *padding) {
	if (cmd->flags & long_display) {
		for (int i = 0; i < parent->nb_childs; i++) {
			calculate_padding(padding, parent->childs[i]);
		}
		return ;
	}

	int total_len = ((parent->longest_el + 2) * parent->nb_childs) - 2;

	padding->n_lines = round_split(total_len, cmd->cols);
	if (!padding->n_lines)
		padding->n_lines = 1;

	padding->n_cols = round_split(parent->nb_childs, padding->n_lines);
	padding->cols = ft_calloc(padding->n_cols, sizeof(Col));

	return ;
}

int	add_to_file_system(File *parent, struct dirent *entry, bool long_display) {
	File **new_file_system = malloc((parent->nb_childs + 1) * sizeof(File*));

	for (int i = 0; i < parent->nb_childs; i++) {
		new_file_system[i] = parent->childs[i];
	}
	free(parent->childs);
	parent->childs = new_file_system;

	File *new_entry = ft_calloc(1, sizeof(File));
	new_file_system[parent->nb_childs] = new_entry;
	parent->nb_childs++;

	new_entry->name = ft_strlen(parent->path);
	if (ft_strcmp("/", parent->path))
		new_entry->name++;

	if (ft_strcmp(parent->path, "/"))
		new_entry->path = join_with_separator(parent->path, entry->d_name, '/');
	else
		new_entry->path = ft_strjoin(parent->path, entry->d_name);

	if (entry->d_type == DT_DIR)
		new_entry->type = DIRECTORY;
	else if (entry->d_type == DT_LNK)
		new_entry->type = SYMLINK;
	else if (entry->d_type == DT_CHR)
		new_entry->type = CHARACTER;
	else if (entry->d_type == DT_BLK)
		new_entry->type = BLOCK;
	else if (entry->d_type == DT_FIFO)
		new_entry->type = PIPE;
	else if (entry->d_type == DT_SOCK)
		new_entry->type = SOCKET;
	else
		new_entry->type = REGULAR_FILE;

	int len = ft_strlen(NAME(new_entry));
	if (parent->longest_el < len)
		parent->longest_el = len;

	if (!analyze_file(new_entry, long_display))
		return 0;
	if (!long_display || new_entry->error)
		return 1;
	parent->total += new_entry->blocks / 2;
	return 1;
}
