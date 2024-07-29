#include "header.h"

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
